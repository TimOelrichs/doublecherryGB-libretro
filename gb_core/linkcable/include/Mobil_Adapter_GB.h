#pragma once

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

// Libretro-Common Sockets (Plattformunabhängig)
extern "C" {
    #include "../libretro-common/include/compat/net/net_socket.h"
    #include "../libretro-common/include/compat/net/net_compat.h"
    #include "../extern/libmobile/mobile.h"
}

#include "../gb_core/gb.h"

class Mobile_Adapter_GB : public I_linkcable_target
{
public:
    // Konstruktor mit shared_ptr für den GB-Core
    explicit Mobile_Adapter_GB(std::shared_ptr<gb> target_gb, std::string configPath = "mobile_adapter_gb.cfg")
        : m_configPath(std::move(configPath)), connected_gb(std::move(target_gb)) {

        m_config.resize(MOBILE_CONFIG_SIZE, 0);
        loadConfig();

        m_adapter = mobile_new(this);
        setupCallbacks();

        start();
    }

    ~Mobile_Adapter_GB() override
    {
        if (m_adapter) {
            mobile_stop(m_adapter);
            free(m_adapter);
        }
        saveConfig();

        // Sockets schließen beim Beenden
        for (auto const& [id, sock] : m_sockets) {
            if (sock.fd != -1) socket_close(sock.fd);
        }
    }

    // Byte-Transfer vom Link-Kabel
    byte receive_from_linkcable(byte data) override {
        return mobile_transfer(m_adapter, data);
    };

    void start() { mobile_start(m_adapter); }
    void stop()  { mobile_stop(m_adapter); }

    // Muss im Emulator-Loop gerufen werden (z.B. 1x pro Frame)
    void update() {
        mobile_loop(m_adapter);

        // Aktionen der Library abarbeiten (Config speichern, Sockets etc.)
        mobile_action action = mobile_actions_get(m_adapter);
        while (action != MOBILE_ACTION_NONE) {
            mobile_actions_process(m_adapter, action);
            action = mobile_actions_get(m_adapter);
        }
    }

private:
    struct mobile_adapter* m_adapter = nullptr;
    std::string m_configPath;
    std::vector<uint8_t> m_config;

    // Timer basierend auf GB-Zyklen
    std::map<unsigned, uint32_t> m_clock_latches;

    std::shared_ptr<gb> connected_gb;
    struct Socket { int fd = -1; };
    std::map<unsigned, Socket> m_sockets;

    void setupCallbacks() {
        // 1. Debug Log
        mobile_def_debug_log(m_adapter, [](void* u, const char* l) {
            std::cout << "[Mobile] " << l << std::endl;
        });

        // 2. Config Read/Write (via m_config Buffer)
        mobile_def_config_read(m_adapter, [](void* u, void* d, uintptr_t o, size_t s) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            if (o + s > MOBILE_CONFIG_SIZE) return false;
            std::memcpy(d, self->m_config.data() + o, s);
            return true;
        });

        mobile_def_config_write(m_adapter, [](void* u, const void* src, uintptr_t o, size_t s) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            if (o + s > MOBILE_CONFIG_SIZE) return false;
            std::memcpy(self->m_config.data() + o, src, s);
            return true;
        });

        // 3. Zeit-Logik via TGBDual total_clock
        mobile_def_time_latch(m_adapter, [](void* u, unsigned t) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            self->m_clock_latches[t] = (uint32_t)self->connected_gb->total_clock;
        });

        mobile_def_time_check_ms(m_adapter, [](void* u, unsigned t, unsigned ms) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);
            uint32_t diff = (uint32_t)self->connected_gb->total_clock - self->m_clock_latches[t];
            // 4194 Ticks pro ms bei ~4.19MHz
            return diff >= (ms * 4194);
        });

        // 4. Socket Open via Libretro-Common
        mobile_def_sock_open(m_adapter, [](void* u, unsigned c, mobile_socktype t, mobile_addrtype at, unsigned p) {
            auto* self = static_cast<Mobile_Adapter_GB*>(u);

            int domain = (at == MOBILE_ADDRTYPE_IPV6) ? RETRO_AF_INET6 : RETRO_AF_INET;
            int type = (t == MOBILE_SOCKTYPE_TCP) ? RETRO_SOCK_STREAM : RETRO_SOCK_DGRAM;

            int fd = socket_create(domain, type, 0);
            if (fd < 0) return false;

            socket_set_nonblock(fd, true);
            self->m_sockets[c] = { fd };
            return true;
        });

        // Hier müssen noch sock_connect, sock_send, sock_recv rein (analog zum BGB Beispiel)
    }

    void loadConfig() {
        std::ifstream f(m_configPath, std::ios::binary);
        if (f) {
            f.read(reinterpret_cast<char*>(m_config.data()), MOBILE_CONFIG_SIZE);
        } else {
            // Initialer Header falls Datei neu
            m_config[0] = 'M'; m_config[1] = 'A';
        }
    }

    void saveConfig() {
        std::ofstream f(m_configPath, std::ios::binary);
        if (f) f.write(reinterpret_cast<char*>(m_config.data()), MOBILE_CONFIG_SIZE);
    }
};