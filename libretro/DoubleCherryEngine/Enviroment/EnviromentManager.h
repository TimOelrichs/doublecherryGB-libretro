#pragma once
#include "../libretro.h"
#include "../common/interfaces/ISingleton.hpp"
#include <DoubleCherryEngine/EngineCoreManager/ICore.hpp>
#include <DoubleCherryEngine/libretro/libretroVariables.h>


class EnvironmentManager final : public ISingleton<EnvironmentManager> {
    friend class ISingleton<EnvironmentManager>;

public:

    unsigned level = 4;
    const struct retro_log_callback LOG_INTERFACE;
    const struct retro_rumble_interface RUMBLE_INTERFACE;
    const struct retro_led_interface LED_INTERFACE;

   
private:
    template <typename T, typename F>
    void assignInterface(unsigned cmd, T& iface_struct, F& func_ptr, F T::* member_ptr) {
        if (environ_cb(cmd, &iface_struct))
            func_ptr = iface_struct.*member_ptr;
        else
            func_ptr = nullptr;
    }

    EnvironmentManager() {

        assignInterface(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, LOG_INTERFACE, log_cb, &retro_log_callback::log);
        assignInterface(RETRO_ENVIRONMENT_GET_LED_INTERFACE, LED_INTERFACE, led_state_cb, &retro_led_interface::set_led_state);
        assignInterface(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, RUMBLE_INTERFACE, rumble_state_cb, &retro_rumble_interface::set_rumble_state);

        environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);

        if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
            libretro_supports_bitmasks = true;

    }
   
};