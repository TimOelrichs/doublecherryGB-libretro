
#include "./include/alleyway_link_controller.hpp"
#pragma once
#include "../../libretro/libretro.h"

extern retro_input_state_t input_state_cb;
extern bool alleyway_analog_controller_enabled; 
extern void display_message(std::string msg_str);

// Settings
#define PADDLE_SOFTNESS 1.2f  // Strength of curve (higher = softer)
#define PADDLE_CURVE_TYPE 2   // 0 = linear, 1 = quadratic, 2 = cubic
#define PADDLE_BIAS 1.1f      // Bias for faster steering to edges (>1.0 = faster)



byte alleyway_link_controller::get_paddle_value(unsigned port)
{
 

    // Read analog stick X-axis
    int16_t analog_x = input_state_cb(port, RETRO_DEVICE_ANALOG, 0, RETRO_DEVICE_ID_ANALOG_X);

    // Define deadzone (±5% of full range)
    const int16_t deadzone = 32768 * 5 / 100;

    if (analog_x > -deadzone && analog_x < deadzone)
        analog_x = 0;

    // Normalize analog_x from [-32768, +32767] to [0.0, 1.0]
    float norm = (analog_x + 32768) / 65535.0f;

    // Apply optional curve for smoother control
    if (PADDLE_SOFTNESS > 0.0f)
    {
        if (PADDLE_CURVE_TYPE == 1)
        {
            // Quadratic softening
            if (norm < 0.5f)
                norm = 0.5f - (0.5f - norm) * (0.5f - norm) * (1.0f + PADDLE_SOFTNESS);
            else
                norm = 0.5f + (norm - 0.5f) * (norm - 0.5f) * (1.0f + PADDLE_SOFTNESS);
        }
        else if (PADDLE_CURVE_TYPE == 2)
        {
            // Cubic softening
            float t = (norm - 0.5f) * 2.0f; // t ∈ [-1, +1]
            t = t * t * t;                  // cubic curve
            norm = (t * 0.5f + 0.5f);        // back to [0.0, 1.0]
        }
    }

    // Apply bias to make edges slightly faster
    if (PADDLE_BIAS != 1.0f)
    {
        float t = (norm - 0.5f) * 2.0f; // t ∈ [-1, +1]
        t = t * PADDLE_BIAS;
        if (t < -1.0f) t = -1.0f;
        if (t > 1.0f) t = 1.0f;
        norm = (t * 0.5f + 0.5f);       // back to [0.0, 1.0]
    }

    // Map normalized value to valid paddle range [0x3F, 0x97]
    uint8_t paddle = (uint8_t)(0x3F + norm * (0x97 - 0x3F));

    // Clip to ensure within valid range
    if (paddle < 0x3F)
        paddle = 0x3F;
    else if (paddle > 0x97)
        paddle = 0x97;

    return paddle;
}

byte alleyway_link_controller::receive_from_linkcable(byte in_data)
{
 
    if (!alleyway_analog_controller_enabled) return 0xFF; 

    //first byte is current analog posiion 
	if (is_even_transfer) {
        is_even_transfer = !is_even_transfer;
		return get_paddle_value(0);
	}
    is_even_transfer = !is_even_transfer;
    //second byte is Buttonpress (Start, A), but we don't need this, cause the normal buttons still work
	return 0;
}

void alleyway_link_controller::handle_special_hotkey(int key) {

    //SELECT BUTTON
    if (key == 0x10) {
        alleyway_analog_controller_enabled = !alleyway_analog_controller_enabled;

        if(alleyway_analog_controller_enabled)
            display_message("Analogscontroller plugged into linkport");
        else  display_message("Analogscontroller plugged out");
    }
  
   
}