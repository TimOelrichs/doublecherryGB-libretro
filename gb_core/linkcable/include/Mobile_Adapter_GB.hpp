// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <chrono>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <memory>

extern "C" {
#include <libretro.h>
#include "../extern/libmobile/mobile_data.h"
#include "../extern/libmobile/mobile.h"
}

#include "../gb_core/gb.h"

// Konstanten für die Game Boy Color Hardware-Uhr zur Netzwerk-Zeitsynchronisation
static constexpr uint32_t GBC_CPU_CLOCK_NORMAL = 4194304;
static constexpr uint32_t GBC_CPU_CLOCK_DOUBLE = 8388608;
static constexpr uint32_t GBC_TICKS_PER_MS_NORMAL = 4194;
static constexpr uint32_t GBC_TICKS_PER_MS_DOUBLE = 8388;

class Mobile_Adapter_GB : public I_Linkcable_Target
{
public:
    struct SocketState {
        int fd = -1;
    };

    explicit Mobile_Adapter_GB(gb* target_gb, std::string configPath = "mobile_adapter_gb_config.bin");
    ~Mobile_Adapter_GB() override;

    // I_Linkcable_Target Interface
    byte receive_from_linkcable(byte data) override;

    void set_double_speed(bool enabled);
    void start();
    void stop();
    void update();
    void update_options_from_libretro(retro_environment_t environ_cb);

public:
    gb* connected_gb;
    bool m_is_double_speed;
    std::map<unsigned, uint64_t> m_time_micros_latches;
    std::vector<uint8_t> m_config;
    std::string m_configPath;


    std::map<unsigned, SocketState> m_sockets;

private:
    std::string mobile_adapter_gb_custom_server_file_path = "/mobile_adapter_gb_custom_server_config.ini";

    alignas(64) struct mobile_adapter m_adapter;
    alignas(64) std::string m_target_number;
    std::string m_local_number;

    std::string m_dns_ip;
    std::string m_relay_ip;
    enum mobile_adapter_device m_device_color;
    uint16_t m_dns_port;
    uint16_t m_relay_port;
    uint8_t m_slave_SB;
    bool m_use_relay = false;
    bool m_show_own_phonenumber_at_startup;
    bool already_showed_phone_number;
    bool persistent_relay_token_enabled = true;
    bool login_logged = false;

    void setupCallbacks();
    void loadConfig();
    void saveConfig();
    void ensureCustomIniExists(const std::string& systemDir);

    std::string getLocalIPAsPhoneNumber();
    std::string getPublicIPAsPhoneNumber();
};