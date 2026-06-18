// SPDX-License-Identifier: LGPL-3.0-or-later
#include "./include/Mobile_Adapter_GB.hpp"

#include <cstring>
#include <fstream>
#include <stdexcept>



extern "C" {
    #include "../libretro-common/include/net/net_socket.h"
    #include "../libretro-common/include/net/net_compat.h"
    #include "../libretro-common/include/net/net_http.h"
    #include "../libretro-common/include/file/config_file.h"
    #include "../extern/libmobile/mobile_inet.h"
    #include "../extern/libmobile/relay.h"
    #include "../extern/libmobile/callback.h"
}

extern retro_log_printf_t log_cb;
extern retro_environment_t environ_cb;
extern void display_message(std::string msg_str, unsigned int seconds, bool alt);
extern bool mobile_adapter_enabled;

Mobile_Adapter_GB::Mobile_Adapter_GB(gb* target_gb, std::string configPath)
     : connected_gb(target_gb),
       m_is_double_speed(false),
       m_configPath(std::move(configPath)),
       m_device_color(MOBILE_ADAPTER_BLUE),
       m_dns_ip("127.0.0.1"),
       m_relay_ip("127.0.0.1"),
       m_dns_port(53),
       m_relay_port(31227),
       m_slave_SB(0xD2),
       m_use_relay(true)
{

    std::memset(&m_adapter, 0, sizeof(m_adapter));

    if (!network_init()) {
        if (log_cb) log_cb(RETRO_LOG_ERROR, "[Mobile] network_init() failed!\n");
        throw std::runtime_error("network_init() failed");
    }

    if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Initializing Mobile Adapter...\n");

    if (environ_cb) {
        const char* system_dir = nullptr;
        if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) && system_dir) {
            ensureCustomIniExists(system_dir);
        }
    }

    m_config.resize(MOBILE_CONFIG_SIZE, 0);


    mobile_init(&m_adapter, this);
    m_adapter.user = this;
    mobile_relay_init(&m_adapter);

    mobile_callback_init(&m_adapter);

    setupCallbacks();

    loadConfig();

    if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Adapter initialized\n");
    start();
}

Mobile_Adapter_GB::~Mobile_Adapter_GB()
{
    if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Shutting down...\n");

    connected_gb = nullptr;
    mobile_stop(&m_adapter);
    saveConfig();

    for (auto& [id, state] : m_sockets) {
        if (state.fd >= 0) {
            shutdown(state.fd, 2);
            socket_close(state.fd);
            state.fd = -1;
        }
    }
    m_sockets.clear();

}

void Mobile_Adapter_GB::ensureCustomIniExists(const std::string& systemDir)
{

    std::string ini_path = systemDir + mobile_adapter_gb_custom_server_file_path;

    config_file_t* conf = config_file_new(ini_path.c_str());

    if (!conf) {
        if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Custom INI existiert nicht. Erstelle Standard-INI unter: %s\n", ini_path.c_str());

        conf = config_file_new(nullptr);
        if (!conf) return;

        config_set_string(conf, "dns_ip", "127.0.0.1");
        config_set_int(conf, "dns_port", 53);
        config_set_string(conf, "relay_ip", "127.0.0.1");
        config_set_int(conf, "relay_port", 31227);

        if (!config_file_write(conf, ini_path.c_str(), false)) {
            if (log_cb) log_cb(RETRO_LOG_ERROR, "[Mobile] Fehler beim Schreiben der Custom-INI!\n");
        }

        config_file_free(conf);
    } else {
        config_file_free(conf);
    }
}

byte Mobile_Adapter_GB::receive_from_linkcable(byte data)
{
    uint8_t byte_to_return = m_slave_SB;
    m_slave_SB = mobile_transfer(&m_adapter, data);
    return byte_to_return;
}

void Mobile_Adapter_GB::set_double_speed(bool enabled)
{
    m_is_double_speed = enabled;
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[Mobile] Double Speed Mode: %s\n", enabled ? "enabled" : "disabled");
    }
}

void Mobile_Adapter_GB::start() { mobile_start(&m_adapter); }
void Mobile_Adapter_GB::stop()  { mobile_stop(&m_adapter); }

void Mobile_Adapter_GB::update()
{
    if (!connected_gb) return;

    mobile_loop(&m_adapter);

    if (m_use_relay) {
        unsigned char relay_conn = 0;
        struct mobile_addr relay_server_addr = m_adapter.config.relay;
        struct mobile_adapter_relay *s = &m_adapter.relay;


        if (s->state < MOBILE_RELAY_CONNECTED) {
            login_logged = false;
        }


        if (s->state < MOBILE_RELAY_CONNECTED) {
            mobile_relay_proc_init_number(&m_adapter, relay_conn, &relay_server_addr);
        }

        if (s->state == MOBILE_RELAY_CONNECTED && !login_logged) {
            if (log_cb) {
                log_cb(RETRO_LOG_INFO, "[Mobile] Successfully logged in and ready to receive!\n");
            }
            login_logged = true;
        }


        if (s->state == MOBILE_RELAY_CONNECTED) {

            if (!m_target_number.empty()) {
                s->processing = 2;
                int rc = mobile_relay_proc_call(&m_adapter, relay_conn, &relay_server_addr,
                                                m_target_number.c_str(), m_target_number.length());

                if (rc == MOBILE_RELAY_CALL_RESULT_ACCEPTED) {
                    if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Call successfully connected! Tunnel established.\n");
                    m_target_number.clear();
                } else if (rc < 0) {
                    if (log_cb) log_cb(RETRO_LOG_ERROR, "[Mobile] Call failed.\n");
                    m_target_number.clear();
                }
            }
            else {
                s->processing = 2;
                mobile_relay_proc_wait(&m_adapter, relay_conn, &relay_server_addr);
            }
        }

        else if (s->state == MOBILE_RELAY_RECV_WAIT) {
            char peer_number[MOBILE_MAX_NUMBER_SIZE + 1] = {0};
            unsigned peer_number_len = 0;

            int rc = mobile_relay_wait(&m_adapter, relay_conn, peer_number, &peer_number_len);
            if (rc == MOBILE_RELAY_WAIT_RESULT_ACCEPTED) {
                if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] An incoming call was ACCEPTED! Tunnel established.\n");
            }
        }
        else if (s->state == MOBILE_RELAY_RECV_CALL) {
            int rc = mobile_relay_call(&m_adapter, relay_conn, NULL, 0);
            if (rc == MOBILE_RELAY_CALL_RESULT_ACCEPTED) {
                if (log_cb) log_cb(RETRO_LOG_INFO, "[Mobile] Remote peer answered the call! Tunnel established.\n");
            }
        }
    }

    enum mobile_action action = mobile_actions_get(&m_adapter);
    if (action != MOBILE_ACTION_NONE && connected_gb) {
        mobile_actions_process(&m_adapter, action);
    }
}

void Mobile_Adapter_GB::update_options_from_libretro(retro_environment_t environ_cb)
{
    if (!environ_cb) return;

    struct retro_variable var = {0};
    bool changed = false;

    var.key = "dcgb_mobile_adatper_use_relay_server";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        bool target_use_relay = (std::strcmp(var.value, "1") == 0);
        if (m_use_relay != target_use_relay) {
            m_use_relay = target_use_relay;
            changed = true;
        }
    }

    var.key = "dcgb_mobile_adapter_show_p2p_phonenumber";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        bool target_show_phone_number = (std::strcmp(var.value, "1") == 0);
        if (m_show_own_phonenumber_at_startup!= target_show_phone_number) {
            m_show_own_phonenumber_at_startup = target_show_phone_number;
            changed = true;
        }
    }


    bool custom_dns_ip = false;
    var.key = "dcgb_mobile_adatper_dns_ip";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        if (std::strcmp(var.value, "99") == 0) {
            custom_dns_ip = true;
        } else if (m_dns_ip != var.value) {
            m_dns_ip = var.value; // Nimmt direkt die IP aus den Core Options (z.B. "127.0.0.1" oder künftige IPs)
            changed = true;
        }
    }

    bool custom_dns_port = false;
    var.key = "dcgb_mobile_adatper_dns_port";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        if (std::strcmp(var.value, "99") == 0) {
            custom_dns_port = true;
        } else {
            uint16_t target_dns_port = 53;
            if (std::strcmp(var.value, "2") == 0 || std::strcmp(var.value, "1053") == 0) {
                target_dns_port = 1053;
            }

            if (m_dns_port != target_dns_port) {
                m_dns_port = target_dns_port;
                changed = true;
            }
        }
    }

    bool custom_relay_ip = false;
    var.key = "dcgb_mobile_adatper_relay_ip";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        if (std::strcmp(var.value, "99") == 0) {
            custom_relay_ip = true;
        } else if (m_relay_ip != var.value) {
            m_relay_ip = var.value;
            changed = true;
        }
    }

    bool custom_relay_port = false;
    var.key = "dcgb_mobile_adatper_relay_port";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        if (std::strcmp(var.value, "99") == 0) {
            custom_relay_port = true;
        } else {
            uint16_t target_relay_port = 31227;
            // Falls du später einen Port "2" mit einem anderen Wert belegst:
            if (std::strcmp(var.value, "2") == 0) {
                target_relay_port = 12345; // Beispiel für zukünftige Erweiterung
            }

            if (m_relay_port != target_relay_port) {
                m_relay_port = target_relay_port;
                changed = true;
            }
        }
    }

    // --- 6. Device Color ---
    var.key = "dcgb_mobile_adatper_device_color";
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        enum mobile_adapter_device target_device = (enum mobile_adapter_device)std::atoi(var.value);
        if (m_device_color != target_device) {
            m_device_color = target_device;
            changed = true;
        }
    }

    // --- 7. Custom INI Overrides ---
    if (custom_dns_ip || custom_dns_port || custom_relay_ip || custom_relay_port) {
        const char* system_dir = nullptr;
        if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) && system_dir) {

            std::string ini_path = std::string(system_dir) + mobile_adapter_gb_custom_server_file_path;

            config_file_t* conf = config_file_new(ini_path.c_str());
            if (conf) {
                char tmp_str[256];

                if (custom_dns_ip && config_get_array(conf, "dns_ip", tmp_str, sizeof(tmp_str))) {
                    if (m_dns_ip != tmp_str) { m_dns_ip = tmp_str; changed = true; }
                }

                int tmp_int = 0;
                if (custom_dns_port && config_get_int(conf, "dns_port", &tmp_int)) {
                    if (m_dns_port != (uint16_t)tmp_int) { m_dns_port = (uint16_t)tmp_int; changed = true; }
                }

                if (custom_relay_ip && config_get_array(conf, "relay_ip", tmp_str, sizeof(tmp_str))) {
                    if (m_relay_ip != tmp_str) { m_relay_ip = tmp_str; changed = true; }
                }

                if (custom_relay_port && config_get_int(conf, "relay_port", &tmp_int)) {
                    if (m_relay_port != (uint16_t)tmp_int) { m_relay_port = (uint16_t)tmp_int; changed = true; }
                }

                config_file_free(conf);
            } else {
                if (log_cb) {
                    log_cb(RETRO_LOG_WARN, "[Mobile] Custom-Option active, but could not load %s!\n", ini_path.c_str());
                }
            }
        }
    }

    if (changed) {
        loadConfig();
    }
}

/*
void Mobile_Adapter_GB::setupCallbacks()
{
    mobile_def_debug_log(&m_adapter, [](void* u, const char* line) {
        if (log_cb) log_cb(RETRO_LOG_DEBUG, "[libmobile] %s\n", line);
    });

    mobile_def_config_read(&m_adapter, [](void* u, void* d, uintptr_t o, size_t s) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (!self) return false;
        if (o + s > MOBILE_CONFIG_SIZE) return false;
        std::memcpy(d, self->m_config.data() + o, s);
        return true;
    });

    mobile_def_config_write(&m_adapter, [](void* u, const void* src, uintptr_t o, size_t s) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (!self) return false;
        if (o + s > MOBILE_CONFIG_SIZE) return false;
        std::memcpy(self->m_config.data() + o, src, s);
        return true;
    });

    mobile_def_time_latch(&m_adapter, [](void* u, unsigned t) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (self && self->connected_gb) {
            self->m_time_micros_latches[t] = self->connected_gb->get_cpu()->get_clock();
        }
    });

    mobile_def_time_check_ms(&m_adapter, [](void* u, unsigned t, unsigned ms) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (!self || !self->connected_gb) return true;

        uint64_t current_cycles = self->connected_gb->get_cpu()->get_clock();
        uint64_t latched_cycles = self->m_time_micros_latches[t];

        if (current_cycles < latched_cycles) return true;

        uint64_t elapsed_cycles = current_cycles - latched_cycles;
        uint64_t ticks_per_ms = self->m_is_double_speed ? GBC_TICKS_PER_MS_DOUBLE : GBC_TICKS_PER_MS_NORMAL;
        return elapsed_cycles > ((uint64_t)ms * ticks_per_ms);
    });

    mobile_def_sock_open(&m_adapter, [](void* u, unsigned c, enum mobile_socktype t,
                                       enum mobile_addrtype at, unsigned p) {
        // SEGFAULT-SCHUTZ: Fallback auf globalen Context, falls Library u verfälscht
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return false;

        enum socket_type sock_type = (t == MOBILE_SOCKTYPE_TCP) ? SOCKET_TYPE_STREAM : SOCKET_TYPE_DATAGRAM;
        enum socket_protocol protocol = (t == MOBILE_SOCKTYPE_TCP) ? SOCKET_PROTOCOL_TCP : SOCKET_PROTOCOL_UDP;

        int fd = socket_create("mobile", SOCKET_DOMAIN_INET, sock_type, protocol);
        if (fd < 0) return false;

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
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return;
        auto it = self->m_sockets.find(c);
        if (it != self->m_sockets.end() && it->second.fd >= 0) {
            socket_close(it->second.fd);
            it->second.fd = -1;
        }
    });

    mobile_def_sock_connect(&m_adapter, [](void* u, unsigned c, const struct mobile_addr *addr) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return -1;

        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return -1;

        struct sockaddr_in sa = {};
        if (addr && addr->type == MOBILE_ADDRTYPE_IPV4) {
            const struct mobile_addr4 *a4 = (const struct mobile_addr4 *)addr;
            sa.sin_family = AF_INET;
            sa.sin_port = htons(a4->port);
            std::memcpy(&sa.sin_addr, a4->host, 4);
        } else {
            return -1;
        }

        socket_set_block(it->second.fd, false);
        int rc = connect(it->second.fd, (struct sockaddr *)&sa, sizeof(sa));

        if (rc == 0) return 1;

        int err = errno;
        if (err == EINPROGRESS || err == EWOULDBLOCK || err == EALREADY) {
            fd_set wr_fds;
            FD_ZERO(&wr_fds);
            FD_SET(it->second.fd, &wr_fds);

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 500;

            int select_rc = select(it->second.fd + 1, NULL, &wr_fds, NULL, &tv);
            if (select_rc > 0) {
                int so_error = 0;
                socklen_t len = sizeof(so_error);
                getsockopt(it->second.fd, SOL_SOCKET, SO_ERROR, &so_error, &len);
                if (so_error == 0) return 1;
                return -1;
            }
            return 0;
        }

        if (err == EISCONN) return 1;
        return -1;
    });

    mobile_def_sock_listen(&m_adapter, [](void* u, unsigned c) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return false;
        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return false;
        return listen(it->second.fd, 1) == 0;
    });

    mobile_def_sock_accept(&m_adapter, [](void* u, unsigned c) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return false;
        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return false;

        bool rd = false, wr = false;
        if (!socket_wait(it->second.fd, &rd, &wr, 0) || !rd) return false;

        int newsock = accept(it->second.fd, NULL, NULL);
        if (newsock < 0) return false;

        socket_set_block(newsock, false);
        socket_close(it->second.fd);
        it->second.fd = newsock;
        return true;
    });

    mobile_def_sock_send(&m_adapter, [](void* u, unsigned c, const void *d, unsigned s,
                               const struct mobile_addr *a) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return -1;
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
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return -1;

        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return 0;

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(it->second.fd, &read_fds);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000;

        int select_rc = select(it->second.fd + 1, &read_fds, NULL, NULL, &tv);
        if (select_rc <= 0) return 0;

        struct sockaddr_in sa = {};
        socklen_t from_len = sizeof(sa);

        ssize_t len = recvfrom(it->second.fd, (char*)d, s, 0, (struct sockaddr*)&sa, &from_len);

        if (len < 0) {
            return (errno == EWOULDBLOCK || errno == EAGAIN) ? 0 : -1;
        }
        if (len == 0) return -2;

        if (a && from_len >= sizeof(sa) && sa.sin_family == AF_INET) {
            a->type = MOBILE_ADDRTYPE_IPV4;
            struct mobile_addr4 *a4 = (struct mobile_addr4 *)a;

            uint16_t incoming_port = ntohs(sa.sin_port);
            uint8_t* real_ip = (uint8_t*)&sa.sin_addr.s_addr;

            if (log_cb && incoming_port == self->m_dns_port) {
                log_cb(RETRO_LOG_INFO, "[Mobile DNS] Packet received (%d Bytes) from %d.%d.%d.%d:%d\n",
                       (int)len, real_ip[0], real_ip[1], real_ip[2], real_ip[3], incoming_port);
            }

            a4->port = incoming_port;
            std::memcpy(a4->host, &sa.sin_addr.s_addr, 4);

            if (c != 0 && a4->port == self->m_relay_port) {
                a4->port = self->m_relay_port;
                if (mobile_inet_pton(MOBILE_INET_PTON_ANY, self->m_relay_ip.c_str(), a4->host) != 1) {
                    a4->host[0] = 127; a4->host[1] = 0; a4->host[2] = 0; a4->host[3] = 1;
                }
            }
        }
        return (int)len;
    });

    mobile_def_serial_enable(&m_adapter, [](void* u, bool m) {});
    mobile_def_serial_disable(&m_adapter, [](void* u) {});

    mobile_def_update_number(&m_adapter, [](void* user, enum mobile_number t, const char *number) {
          auto* self = user ? static_cast<Mobile_Adapter_GB*>(user) : nullptr;

          if (number && log_cb) {
              log_cb(RETRO_LOG_INFO, "[Mobile] %s-number updated: %s\n",
                     t == MOBILE_NUMBER_USER ? "User" : "Peer", number);

                 std::string raw_number = number;

                 if (raw_number.length() > 4)
                     raw_number.insert(4, " ");

                 std::string phonenumber = "Your relay Phonenumber:   " + raw_number;
                 display_message(phonenumber, 9, false);
          }


          if (self && t == MOBILE_NUMBER_USER && number) {
              self->m_local_number = number;
          }
      });
}
*/


void Mobile_Adapter_GB::setupCallbacks()
{
    // --- 1. DEBUG LOGGING ---
    mobile_def_debug_log(&m_adapter, [](void* u, const char* line) {
        if (log_cb) log_cb(RETRO_LOG_DEBUG, "[libmobile] %s\n", line);
    });

    // --- 2. CONFIGURATION READ/WRITE ---
    mobile_def_config_read(&m_adapter, [](void* u, void* d, uintptr_t o, size_t s) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (!self) return false;
        if (o + s > MOBILE_CONFIG_SIZE) return false;
        std::memcpy(d, self->m_config.data() + o, s);
        return true;
    });

    mobile_def_config_write(&m_adapter, [](void* u, const void* src, uintptr_t o, size_t s) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (!self) return false;
        if (o + s > MOBILE_CONFIG_SIZE) return false;
        std::memcpy(self->m_config.data() + o, src, s);
        return true;
    });

    // --- 3. TIMING & TIMEOUTS ---
    mobile_def_time_latch(&m_adapter, [](void* u, unsigned t) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (self && self->connected_gb) {
            self->m_time_micros_latches[t] = self->connected_gb->get_cpu()->get_clock();
        }
    });

    mobile_def_time_check_ms(&m_adapter, [](void* u, unsigned t, unsigned ms) {
        auto* self = static_cast<Mobile_Adapter_GB*>(u);
        if (!self || !self->connected_gb) return true;

        uint64_t current_cycles = self->connected_gb->get_cpu()->get_clock();
        uint64_t latched_cycles = self->m_time_micros_latches[t];

        if (current_cycles < latched_cycles) return true;

        uint64_t elapsed_cycles = current_cycles - latched_cycles;
        uint64_t ticks_per_ms = self->m_is_double_speed ? GBC_TICKS_PER_MS_DOUBLE : GBC_TICKS_PER_MS_NORMAL;
        return elapsed_cycles > ((uint64_t)ms * ticks_per_ms);
    });

    // --- 4. SOCKET MANAGEMENT ---
    mobile_def_sock_open(&m_adapter, [](void* u, unsigned c, enum mobile_socktype t,
                                       enum mobile_addrtype at, unsigned p) {
        // SEGFAULT PROTECTION: Fallback to global context if library corrupts u
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return false;

        enum socket_type sock_type = (t == MOBILE_SOCKTYPE_TCP) ? SOCKET_TYPE_STREAM : SOCKET_TYPE_DATAGRAM;
        enum socket_protocol protocol = (t == MOBILE_SOCKTYPE_TCP) ? SOCKET_PROTOCOL_TCP : SOCKET_PROTOCOL_UDP;

        int fd = socket_create("mobile", SOCKET_DOMAIN_INET, sock_type, protocol);
        if (fd < 0) return false;

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
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return;
        auto it = self->m_sockets.find(c);
        if (it != self->m_sockets.end() && it->second.fd >= 0) {
            socket_close(it->second.fd);
            it->second.fd = -1;
        }
    });

    mobile_def_sock_connect(&m_adapter, [](void* u, unsigned c, const struct mobile_addr *addr) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return -1;

        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return -1;

        struct sockaddr_in sa = {};
        if (addr && addr->type == MOBILE_ADDRTYPE_IPV4) {
            const struct mobile_addr4 *a4 = (const struct mobile_addr4 *)addr;
            sa.sin_family = AF_INET;
            sa.sin_port = htons(a4->port);
            std::memcpy(&sa.sin_addr, a4->host, 4); // Fixed the 'a44' typo here
        } else {
            return -1;
        }

        socket_set_block(it->second.fd, false);
        int rc = connect(it->second.fd, (struct sockaddr *)&sa, sizeof(sa));

        if (rc == 0) return 1;

        int err = errno;
        if (err == EINPROGRESS || err == EWOULDBLOCK || err == EALREADY) {
            fd_set wr_fds;
            FD_ZERO(&wr_fds);
            FD_SET(it->second.fd, &wr_fds);

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 500;

            int select_rc = select(it->second.fd + 1, NULL, &wr_fds, NULL, &tv);
            if (select_rc > 0) {
                int so_error = 0;
                socklen_t len = sizeof(so_error);
                getsockopt(it->second.fd, SOL_SOCKET, SO_ERROR, &so_error, &len);
                if (so_error == 0) return 1;
                return -1;
            }
            return 0;
        }

        if (err == EISCONN) return 1;
        return -1;
    });

    mobile_def_sock_listen(&m_adapter, [](void* u, unsigned c) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return false;
        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return false;
        return listen(it->second.fd, 1) == 0;
    });

    mobile_def_sock_accept(&m_adapter, [](void* u, unsigned c) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return false;
        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return false;

        bool rd = false, wr = false;
        if (!socket_wait(it->second.fd, &rd, &wr, 0) || !rd) return false;

        int newsock = accept(it->second.fd, NULL, NULL);
        if (newsock < 0) return false;

        socket_set_block(newsock, false);
        socket_close(it->second.fd);
        it->second.fd = newsock;
        return true;
    });

    mobile_def_sock_send(&m_adapter, [](void* u, unsigned c, const void *d, unsigned s,
                               const struct mobile_addr *a) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return -1;
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
        return static_cast<int>(len); // Optimized: Clean 64-to-32-bit cast preventing compiler warnings
    });

    mobile_def_sock_recv(&m_adapter, [](void* u, unsigned c, void *d, unsigned s,
                                       struct mobile_addr *a) {
        auto* self = u ? static_cast<Mobile_Adapter_GB*>(u) : nullptr;
        if (!self) return -1;

        auto it = self->m_sockets.find(c);
        if (it == self->m_sockets.end() || it->second.fd < 0) return 0;

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(it->second.fd, &read_fds);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0; //1000;

        int select_rc = select(it->second.fd + 1, &read_fds, NULL, NULL, &tv);
        if (select_rc <= 0) return 0;

        struct sockaddr_in sa = {};
        socklen_t from_len = sizeof(sa);

        ssize_t len = recvfrom(it->second.fd, (char*)d, s, 0, (struct sockaddr*)&sa, &from_len);

        if (len < 0) {
            return (errno == EWOULDBLOCK || errno == EAGAIN) ? 0 : -1;
        }
        if (len == 0) return -2;

        if (a && from_len >= sizeof(sa) && sa.sin_family == AF_INET) {
            a->type = MOBILE_ADDRTYPE_IPV4;
            struct mobile_addr4 *a4 = (struct mobile_addr4 *)a;

            uint16_t incoming_port = ntohs(sa.sin_port);
            uint8_t* real_ip = (uint8_t*)&sa.sin_addr.s_addr;

            if (log_cb && incoming_port == self->m_dns_port) {
                log_cb(RETRO_LOG_INFO, "[Mobile DNS] Packet received (%d Bytes) from %d.%d.%d.%d:%d\n",
                       (int)len, real_ip[0], real_ip[1], real_ip[2], real_ip[3], incoming_port);
            }

            a4->port = incoming_port;
            std::memcpy(a4->host, &sa.sin_addr.s_addr, 4);

            if (c != 0 && a4->port == self->m_relay_port) {
                a4->port = self->m_relay_port;
                if (mobile_inet_pton(MOBILE_INET_PTON_ANY, self->m_relay_ip.c_str(), a4->host) != 1) {
                    a4->host[0] = 127; a4->host[1] = 0; a4->host[2] = 0; a4->host[3] = 1;
                }
            }
        }
        return static_cast<int>(len); // Optimized: Clean 64-to-32-bit cast preventing compiler warnings
    });

    // Unused serial hardware callbacks (handled within the emulator core layer)
    mobile_def_serial_enable(&m_adapter, [](void* u, bool m) {});
    mobile_def_serial_disable(&m_adapter, [](void* u) {});

    // --- 5. ASYNC NOTIFICATIONS & TELEPHONE NUMBERS ---
    mobile_def_update_number(&m_adapter, [](void* user, enum mobile_number t, const char *number) {
          auto* self = user ? static_cast<Mobile_Adapter_GB*>(user) : nullptr;

          if (number && log_cb) {
              log_cb(RETRO_LOG_INFO, "[Mobile] %s-number updated: %s\n",
                     t == MOBILE_NUMBER_USER ? "User" : "Peer", number);

                 std::string raw_number = number;

                 if (raw_number.length() > 4)
                     raw_number.insert(4, " ");

              if (self && self->m_use_relay) {
                  std::string phonenumber = "Your relay Phonenumber:   " + raw_number;
                  display_message(phonenumber, 9, false);
              }
          }

          if (self && t == MOBILE_NUMBER_USER && number) {
              self->m_local_number = number;
          }
      });
}

void Mobile_Adapter_GB::loadConfig()
{

    if (m_show_own_phonenumber_at_startup && !already_showed_phone_number) {

        if (!m_use_relay) {
            std::string my_local_ip = "Your local Phonenumber:   "  + getLocalIPAsPhoneNumber();
            std::string my_public_ip = "Your public Phonenumber:   " + getPublicIPAsPhoneNumber();

            display_message(my_local_ip, 9, false);
            display_message(my_public_ip, 9, false);
            already_showed_phone_number = true;
        }

    }

    m_config.assign(MOBILE_CONFIG_SIZE, 0);

    std::ifstream f(m_configPath, std::ios::binary);
    if (f) f.read(reinterpret_cast<char*>(m_config.data()), MOBILE_CONFIG_SIZE);

    mobile_config_load(&m_adapter);

    enum mobile_adapter_device device = m_device_color;
    if (device < MOBILE_ADAPTER_BLUE || device > MOBILE_ADAPTER_RED) {
        device = MOBILE_ADAPTER_BLUE;
    }
    mobile_config_set_device(&m_adapter, device, false);

    struct mobile_addr dns1 = {};
    dns1.type = MOBILE_ADDRTYPE_IPV4;
    dns1._addr4.port = m_dns_port;
    mobile_inet_pton(MOBILE_INET_PTON_ANY, m_dns_ip.c_str(), dns1._addr4.host);
    mobile_config_set_dns(&m_adapter, &dns1, MOBILE_DNS1);

    struct mobile_addr dns2 = {};
    dns2.type = MOBILE_ADDRTYPE_IPV4;
    dns2._addr4.port = m_dns_port;
    mobile_inet_pton(MOBILE_INET_PTON_ANY, m_dns_ip.c_str(), dns2._addr4.host);
    mobile_config_set_dns(&m_adapter, &dns2, MOBILE_DNS2);

    mobile_config_set_p2p_port(&m_adapter, MOBILE_DEFAULT_P2P_PORT);

    if (!m_use_relay) {
        struct mobile_addr no_relay = {};
        mobile_config_set_relay(&m_adapter, &no_relay);
    } else {
        struct mobile_addr relay = {};
        relay.type = MOBILE_ADDRTYPE_IPV4;
        relay._addr4.port = m_relay_port;
        mobile_inet_pton(MOBILE_INET_PTON_ANY, m_relay_ip.c_str(), relay._addr4.host);
        mobile_config_set_relay(&m_adapter, &relay);


        if (persistent_relay_token_enabled) {
            unsigned char saved_token[MOBILE_RELAY_TOKEN_SIZE];
            if (mobile_config_get_relay_token(&m_adapter, saved_token)) {
                mobile_config_set_relay_token(&m_adapter, saved_token);
            }
        }
        else
            mobile_config_set_relay_token(&m_adapter, nullptr);
    }
    mobile_config_save(&m_adapter);
}

void Mobile_Adapter_GB::saveConfig()
{
    std::ofstream f(m_configPath, std::ios::binary);
    if (f) f.write(reinterpret_cast<char*>(m_config.data()), MOBILE_CONFIG_SIZE);
}

std::string Mobile_Adapter_GB::getLocalIPAsPhoneNumber()
{
    std::string fallbackNumber = "000 000 000 000";

    // Erstelle einen UDP-Socket über libretro-common
    int sock = socket_create("mobile_local_ip", SOCKET_DOMAIN_INET, SOCKET_TYPE_DATAGRAM, SOCKET_PROTOCOL_UDP);
    if (sock < 0) return fallbackNumber;

    struct sockaddr_in GoogleDNS = {};
    GoogleDNS.sin_family = AF_INET;
    GoogleDNS.sin_port = htons(53);
    GoogleDNS.sin_addr.s_addr = htonl(0x08080808); // 8.8.8.8

    if (connect(sock, (struct sockaddr*)&GoogleDNS, sizeof(GoogleDNS)) >= 0) {
        struct sockaddr_in localAddr = {};
        socklen_t addrLen = sizeof(localAddr);

        if (getsockname(sock, (struct sockaddr*)&localAddr, &addrLen) == 0) {
            char phoneBuffer[16]; // Platz für "XXX XXX XXX XXX\0"
            uint8_t* ip = (uint8_t*)&localAddr.sin_addr.s_addr;

            // %03d erzwingt 3 Stellen mit führenden Nullen
            snprintf(phoneBuffer, sizeof(phoneBuffer), "%03d %03d %03d %03d",
                     ip[0], ip[1], ip[2], ip[3]);

            socket_close(sock);
            return std::string(phoneBuffer);
        }
    }

    socket_close(sock);
    return fallbackNumber;
}

std::string Mobile_Adapter_GB::getPublicIPAsPhoneNumber()
{
    std::string fallbackNumber = "000 000 000 000";

    // 1. Create a new HTTP connection structure via libretro-common
    // net_http handles URL parsing and platform-native DNS resolution automatically
    struct http_connection_t *conn = net_http_connection_new("http://api.ipify.org", "GET", nullptr);
    if (!conn) {
        return fallbackNumber;
    }

    // 2. Establish connection and resolve DNS asynchronously
    while (!net_http_connection_done(conn)) {
        if (!net_http_connection_iterate(conn)) {
            // Connection failed due to network timeout or DNS failure
            net_http_connection_free(conn);
            return fallbackNumber;
        }
    }

    // 3. Instantiate the HTTP state client
    struct http_t *http_state = net_http_new(conn);
    if (!http_state) {
        net_http_connection_free(conn);
        return fallbackNumber;
    }

    // 4. Update loop to pull the actual data chunk from the network
    size_t progress = 0;
    size_t total = 0;
    while (!net_http_update(http_state, &progress, &total)) {
        // Continuous polling until the response transfer is fully done
    }

    std::string phone_result = fallbackNumber;

    // 5. Verify HTTP transmission success (Status 200 OK)
    if (!net_http_error(http_state) && net_http_status(http_state) == 200) {
        size_t response_len = 0;
        // Extract the downloaded plain text IP body
        uint8_t *data = net_http_data(http_state, &response_len, false);

        if (data && response_len > 0) {
            // Construct a standard string from raw network bytes
            std::string rawIpStr(reinterpret_cast<char*>(data), response_len);

            int ip[4];
            // Format the plain text IP into the Game Boy's 12-digit phone number layout
            if (sscanf(rawIpStr.c_str(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) == 4) {
                char phoneBuffer[16];
                snprintf(phoneBuffer, sizeof(phoneBuffer), "%03d %03d %03d %03d",
                         ip[0], ip[1], ip[2], ip[3]);
                phone_result = std::string(phoneBuffer);
            }
        }
    }

    // 6. Complete standard API cleanup
    net_http_delete(http_state);
    net_http_connection_free(conn);

    return phone_result;
}