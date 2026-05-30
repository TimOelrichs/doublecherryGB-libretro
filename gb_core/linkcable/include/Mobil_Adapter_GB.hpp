#pragma once

#include <chrono>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>

extern "C" {
    #include <libretro.h>
    #include "../libretro-common/include/net/net_socket.h"
    #include "../libretro-common/include/net/net_compat.h"
    #include "../extern/libmobile/mobile_data.h"
    #include "../extern/libmobile/mobile.h"
    #include "../extern/libmobile/mobile_inet.h"
}

#include "../gb_core/gb.h"

extern retro_log_printf_t log_cb;

// Game Boy Color hardware clock constants used for network time syncing
static constexpr uint32_t GBC_CPU_CLOCK_NORMAL = 4194304;
static constexpr uint32_t GBC_CPU_CLOCK_DOUBLE = 8388608;
static constexpr uint32_t GBC_TICKS_PER_MS_NORMAL = 4194;
static constexpr uint32_t GBC_TICKS_PER_MS_DOUBLE = 8388;

class Mobile_Adapter_GB : public I_Linkcable_Target
{
public:
    explicit Mobile_Adapter_GB(gb* target_gb,
                                std::string configPath = "mobile_adapter_gb_config.bin")
         : connected_gb(target_gb),
           m_is_double_speed(false),
           m_configPath(std::move(configPath)) {

        if (!network_init()) {
            if (log_cb) log_cb(RETRO_LOG_ERROR, "[Mobile] network_init() failed!\n");
            throw std::runtime_error("network_init() failed");
        }

        if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Initializing Mobile Adapter...\n");

        m_config.resize(MOBILE_CONFIG_SIZE, 0);

        // Pass 'this' as user pointer context for all upcoming libmobile callbacks
        mobile_init(&m_adapter, this);

        setupCallbacks();
        loadConfig();

        if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Adapter initialized\n");
        start();
    }

    ~Mobile_Adapter_GB() override {
        if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Shutting down...\n");

        /*
        mobile_def_debug_log(&m_adapter, nullptr);
        mobile_def_config_read(&m_adapter, nullptr);
        mobile_def_config_write(&m_adapter, nullptr);
        mobile_def_time_latch(&m_adapter, nullptr);
        mobile_def_time_check_ms(&m_adapter, nullptr);
        mobile_def_sock_open(&m_adapter, nullptr);
        mobile_def_sock_close(&m_adapter, nullptr);
        mobile_def_sock_connect(&m_adapter, nullptr);
        mobile_def_sock_listen(&m_adapter, nullptr);
        mobile_def_sock_accept(&m_adapter, nullptr);
        mobile_def_sock_send(&m_adapter, nullptr);
        mobile_def_sock_recv(&m_adapter, nullptr);
        mobile_def_serial_enable(&m_adapter, nullptr);
        mobile_def_serial_disable(&m_adapter, nullptr);
        mobile_def_update_number(&m_adapter, nullptr);
        */

        // 1. Zuerst libmobile stoppen, damit keine neuen Netzwerk-Aktionen getriggert werden
        connected_gb = nullptr;
        mobile_stop(&m_adapter);
        saveConfig();

        // 2. Sockets explizit disconnecten und schließen
        for (auto& [id, state] : m_sockets) {
            if (state.fd >= 0) {
                // SHUT_RDWR (2) signalisiert dem OS: Keine Transfers mehr erlauben.
                // Das beendet jeden hängenden socket_wait() oder accept() Zustand.
                shutdown(state.fd, 2);
                socket_close(state.fd);
                state.fd = -1;
            }
        }
        m_sockets.clear();
    }

    byte receive_from_linkcable(byte data) override {
        // Hardware Double-Buffering Emulation:
        // Returns the latched byte from the previous transfer while processing
        // the current incoming byte into the libmobile state machine.
        uint8_t byte_to_return = m_slave_SB;
        m_slave_SB = mobile_transfer(&m_adapter, data);
        return byte_to_return;
    }

    void set_double_speed(bool enabled) {
        m_is_double_speed = enabled;
        if (log_cb) {
            log_cb(RETRO_LOG_INFO, "[Mobile] Double Speed Mode: %s\n", enabled ? "enabled" : "disabled");
        }
    }

    void start() { mobile_start(&m_adapter); }
    void stop()  { mobile_stop(&m_adapter); }

    void update() {

        if (!connected_gb) return;

        mobile_loop(&m_adapter);

        // Process any queued abstract hardware actions requested by libmobile
        enum mobile_action action = mobile_actions_get(&m_adapter);
        while (action != MOBILE_ACTION_NONE) {

            if (!connected_gb) return;
            mobile_actions_process(&m_adapter, action);
            action = mobile_actions_get(&m_adapter);
        }
    }

public:
    gb* connected_gb;
    bool m_is_double_speed;
    std::map<unsigned, uint64_t> m_time_micros_latches;
    std::vector<uint8_t> m_config;
    std::string m_configPath;
    uint8_t m_slave_SB = 0xD2; // Default idle byte for the Mobile Adapter serial interface

    struct SocketState {
        int fd = -1;
    };
    std::map<unsigned, SocketState> m_sockets;

private:
    struct mobile_adapter m_adapter;

    void setupCallbacks() {
        mobile_def_debug_log(&m_adapter, [](void* u, const char* line) {
            if (log_cb) log_cb(RETRO_LOG_DEBUG, "[libmobile] %s\n", line);
        });

        mobile_def_config_read(&m_adapter, [](void* u, void* d, uintptr_t o, size_t s) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            if (o + s > MOBILE_CONFIG_SIZE) return false;
            std::memcpy(d, self->m_config.data() + o, s);
            return true;
        });

        mobile_def_config_write(&m_adapter, [](void* u, const void* src, uintptr_t o, size_t s) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            if (o + s > MOBILE_CONFIG_SIZE) return false;
            std::memcpy(self->m_config.data() + o, src, s);
            return true;
        });

        // Cycle-Based Timing Mechanism:
        // Anchoring libmobile timers directly to the Game Boy's CPU clock prevents connection
        // drops during fast-forwarding, emulator pausing, or when accessing the RetroArch menu.
        mobile_def_time_latch(&m_adapter, [](void* u, unsigned t) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            if (self->connected_gb) {
                self->m_time_micros_latches[t] = self->connected_gb->get_cpu()->get_clock();
            } else {
                self->m_time_micros_latches[t] = 0;
            }
        });

        mobile_def_time_check_ms(&m_adapter, [](void* u, unsigned t, unsigned ms) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            if (!self->connected_gb) return true;

            uint64_t current_cycles = self->connected_gb->get_cpu()->get_clock();;
            uint64_t latched_cycles = self->m_time_micros_latches[t];

            // Gracefully handle internal core cycle overflows or soft-resets
            if (current_cycles < latched_cycles) {
                return true;
            }

            uint64_t elapsed_cycles = current_cycles - latched_cycles;
            uint64_t ticks_per_ms = self->m_is_double_speed ? GBC_TICKS_PER_MS_DOUBLE : GBC_TICKS_PER_MS_NORMAL;
            uint64_t required_cycles = (uint64_t)ms * ticks_per_ms;
            return elapsed_cycles > required_cycles;
        });

        mobile_def_sock_open(&m_adapter, [](void* u, unsigned c, enum mobile_socktype t,
                                           enum mobile_addrtype at, unsigned p) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);

            enum socket_type sock_type = (t == MOBILE_SOCKTYPE_TCP) ? SOCKET_TYPE_STREAM : SOCKET_TYPE_DATAGRAM;
            enum socket_protocol protocol = (t == MOBILE_SOCKTYPE_TCP) ? SOCKET_PROTOCOL_TCP : SOCKET_PROTOCOL_UDP;

            int fd = socket_create("mobile", SOCKET_DOMAIN_INET, sock_type, protocol);
            if (fd < 0) return false;

            // Force non-blocking sockets to fit libmobile's cooperative multi-tasking architecture
            socket_set_block(fd, false);

            int opt_on = 1;
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt_on, sizeof(int));
            if (t == MOBILE_SOCKTYPE_TCP) {
                setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&opt_on, sizeof(int));
            }

            struct sockaddr_in addr = {};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(p);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);

            if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                socket_close(fd);
                return false;
            }

            self->m_sockets[c] = {fd};
            return true;
        });

        mobile_def_sock_close(&m_adapter, [](void* u, unsigned c) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            auto it = self->m_sockets.find(c);
            if (it != self->m_sockets.end() && it->second.fd >= 0) {
                socket_close(it->second.fd);
                it->second.fd = -1;
            }
        });

        mobile_def_sock_connect(&m_adapter, [](void* u, unsigned c, const struct mobile_addr *addr) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            auto it = self->m_sockets.find(c);
            if (it == self->m_sockets.end() || it->second.fd < 0) return -1;

            struct sockaddr_in sa = {};
            if (addr->type == MOBILE_ADDRTYPE_IPV4) {
                const struct mobile_addr4 *a4 = (const struct mobile_addr4 *)addr;
                sa.sin_family = AF_INET;
                sa.sin_port = htons(a4->port);
                std::memcpy(&sa.sin_addr, a4->host, 4);
            } else {
                return -1;
            }

            int rc = connect(it->second.fd, (struct sockaddr *)&sa, sizeof(sa));
            int err = errno;

            // Handle non-blocking async TCP connection statuses gracefully
            if (rc == 0) return 1; // Connected instantly
            if (err == EINPROGRESS || err == EWOULDBLOCK || err == EALREADY) return 0; // Connection handshake ongoing
            if (err == EISCONN) return 1; // Connection already established successfully
            return -1; // Connection failed
        });

        mobile_def_sock_listen(&m_adapter, [](void* u, unsigned c) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            auto it = self->m_sockets.find(c);
            if (it == self->m_sockets.end() || it->second.fd < 0) return false;
            return listen(it->second.fd, 1) == 0;
        });

        mobile_def_sock_accept(&m_adapter, [](void* u, unsigned c) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            auto it = self->m_sockets.find(c);
            if (it == self->m_sockets.end() || it->second.fd < 0) return false;

            // Use socket_wait poll wrapper to check for incoming client connections asynchronously
            bool rd = false, wr = false;
            if (!socket_wait(it->second.fd, &rd, &wr, 0) || !rd) return false;

            int newsock = accept(it->second.fd, NULL, NULL);
            if (newsock < 0) return false;

            socket_set_block(newsock, false);
            socket_close(it->second.fd); // Replace the internal listen descriptor with the newly accepted payload socket
            it->second.fd = newsock;
            return true;
        });

        mobile_def_sock_send(&m_adapter, [](void* u, unsigned c, const void *d, unsigned s,
                                   const struct mobile_addr *a) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            auto it = self->m_sockets.find(c);
            if (it == self->m_sockets.end() || it->second.fd < 0) return -1;

            struct sockaddr_in sa = {};
            struct sockaddr* to_addr = NULL;
            socklen_t to_len = 0;

            if (a && a->type == MOBILE_ADDRTYPE_IPV4) {
                const struct mobile_addr4 *a4 = (const struct mobile_addr4 *)a;
                sa.sin_family = AF_INET;
                sa.sin_port = htons(a4->port);
                std::memcpy(&sa.sin_addr, a4->host, 4);
                to_addr = (struct sockaddr*)&sa;
                to_len = sizeof(sa);
            }

            ssize_t len = sendto(it->second.fd, (const char*)d, s, 0, to_addr, to_len);
            if (len < 0) return (errno == EWOULDBLOCK || errno == EAGAIN) ? 0 : -1;
            return (int)len;
        });

        mobile_def_sock_recv(&m_adapter, [](void* u, unsigned c, void *d, unsigned s,
                                           struct mobile_addr *a) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            auto it = self->m_sockets.find(c);
            if (it == self->m_sockets.end() || it->second.fd < 0) return -1;

            struct sockaddr_in sa = {};
            socklen_t from_len = sizeof(sa);

            ssize_t len = recvfrom(it->second.fd, (char*)d, s, 0, (struct sockaddr*)&sa, &from_len);

            if (len < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    return 0; // No incoming data buffered yet
                }
                if (log_cb) log_cb(RETRO_LOG_ERROR, "[Mobile Socket] recvfrom Error: %d\n", errno);
                return -1;
            }
            if (len == 0) {
                return -2; // TCP connection was dropped/closed cleanly by remote server
            }

            if (a && from_len >= sizeof(sa) && sa.sin_family == AF_INET) {
                a->type = MOBILE_ADDRTYPE_IPV4;
                struct mobile_addr4 *a4 = (struct mobile_addr4 *)a;

                uint8_t* real_ip = (uint8_t*)&sa.sin_addr.s_addr;
                if (log_cb && a4->port == 1053) {
                    log_cb(RETRO_LOG_INFO, "[Mobile DNS] Packet received (%d Bytes) from %d.%d.%d.%d:%d\n",
                           (int)len, real_ip[0], real_ip[1], real_ip[2], real_ip[3], ntohs(sa.sin_port));
                }

                // DNS Security Trick:
                // libmobile strictly discards DNS response packets if the sender address
                // structure doesn't match the targeted server explicitly.
                // We spoof the sender details here to align with the active loadConfig parameters.
                a4->port = 1053;
                a4->host[0] = 172;
                a4->host[1] = 18;
                a4->host[2] = 0;
                a4->host[3] = 1;
            }

            return (int)len;
        });

        mobile_def_serial_enable(&m_adapter, [](void* u, bool m) {});
        mobile_def_serial_disable(&m_adapter, [](void* u) {});

        mobile_def_update_number(&m_adapter, [](void* u, enum mobile_number t, const char *n) {
            if (log_cb && n) {
                log_cb(RETRO_LOG_INFO, "[Mobile] %s: %s\n", t == MOBILE_NUMBER_USER ? "User" : "Peer", n);
            }
        });
    }

    void loadConfig() {
        // Clear configuration memory block to eliminate garbage memory data
        // if the targeted file path does not exist yet.
        m_config.assign(MOBILE_CONFIG_SIZE, 0);

        std::ifstream f(m_configPath, std::ios::binary);
        if (f) f.read(reinterpret_cast<char*>(m_config.data()), MOBILE_CONFIG_SIZE);

        enum mobile_adapter_device device = MOBILE_ADAPTER_GREEN;
        bool device_unmetered = false;

        struct mobile_addr dns1 = {};
        struct mobile_addr dns2 = {};

        dns1.type = MOBILE_ADDRTYPE_IPV4;
        dns1._addr4.port = 1053;
        mobile_inet_pton(MOBILE_INET_PTON_ANY, "172.18.0.1", dns1._addr4.host);

        dns2.type = MOBILE_ADDRTYPE_IPV4;
        dns2._addr4.port = 1053;
        mobile_inet_pton(MOBILE_INET_PTON_ANY, "172.18.0.1", dns2._addr4.host);

        unsigned p2p_port = MOBILE_DEFAULT_P2P_PORT;
        struct mobile_addr relay = {};
        bool relay_token_update = false;
        unsigned char *relay_token = NULL;

        mobile_config_load(&m_adapter);
        mobile_config_set_device(&m_adapter, device, device_unmetered);
        mobile_config_set_dns(&m_adapter, &dns1, MOBILE_DNS1);
        mobile_config_set_dns(&m_adapter, &dns2, MOBILE_DNS2);
        mobile_config_set_p2p_port(&m_adapter, p2p_port);
        mobile_config_set_relay(&m_adapter, &relay);
        if (relay_token_update) {
            mobile_config_set_relay_token(&m_adapter, relay_token);
        }
        mobile_config_save(&m_adapter);
    }

    void saveConfig() {
        std::ofstream f(m_configPath, std::ios::binary);
        if (f) f.write(reinterpret_cast<char*>(m_config.data()), MOBILE_CONFIG_SIZE);
    }
};