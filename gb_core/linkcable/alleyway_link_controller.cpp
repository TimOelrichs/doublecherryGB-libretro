#include "./include/alleyway_link_controller.hpp"
#include "../../libretro/libretro.h"

extern retro_input_state_t input_state_cb;
extern bool alleyway_analog_controller_enabled; 
extern void display_message(std::string msg_str);
extern int16_t mouse_x;
extern bool current_mouse_click;

extern Alleyway_Controls_Mode AlleywayControlsMode;

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

// Reads mouse X position and maps it to paddle value
uint8_t alleyway_link_controller::get_paddle_from_mouse(unsigned port)
{

    if (AlleywayControlsMode == Alleyway_Controls_Mode::MOUSE)
    {
        static float paddle_pos = 0x78; // start in center

        // Scale speed – adjust factor for sensitivity
        float speed = 0.3f;
        paddle_pos += mouse_x * speed;

        // Clamp to [0x3F, 0x97]
        if (paddle_pos < 0x3F) paddle_pos = 0x3F;
        if (paddle_pos > 0x97) paddle_pos = 0x97;

        return (uint8_t)paddle_pos;
    }

    if (AlleywayControlsMode == Alleyway_Controls_Mode::TOUCHSCREEN)
    {

     
        //Better for Toachscreens
        static int32_t accum_mouse_x = 0;
        accum_mouse_x += mouse_x;

        // Smaller range for faster paddle response
        if (accum_mouse_x < 0)
            accum_mouse_x = 0;
        if (accum_mouse_x > 512)
            accum_mouse_x = 512;

        float norm = accum_mouse_x / 512.0f;

        // Optional: make movement nonlinear (soft curve)
        // norm = powf(norm, 0.9f); // optional tweak

        // Clamp to [0x3F, 0x97]
        uint8_t paddle_value = (uint8_t)(0x3F + norm * (0x97 - 0x3F));

        return paddle_value;
    }

    return 0xFF;
}


byte alleyway_link_controller::receive_from_linkcable(byte in_data)
{
 
    if (AlleywayControlsMode == Alleyway_Controls_Mode::NORMAL) return 0xFF;

    //first byte is current analog posiion 
	if (is_even_transfer) {
        is_even_transfer = false;;

        switch (AlleywayControlsMode)
        {

        case Alleyway_Controls_Mode::ANALOG_STICK:
            return get_paddle_value(0);
        case Alleyway_Controls_Mode::MOUSE:
        case Alleyway_Controls_Mode::TOUCHSCREEN:
            return get_paddle_from_mouse(0);
        case Alleyway_Controls_Mode::NORMAL:
        default: return 0;
        }
	
	}
    is_even_transfer = true;
    //second byte is Buttonpress (Start, A), but we don't need this, cause the normal buttons still work
	return  current_mouse_click && 
            (AlleywayControlsMode == Alleyway_Controls_Mode::MOUSE) ? 0x8E : 0;
}

void alleyway_link_controller::handle_special_hotkey(int key) {

    //SELECT BUTTON
    if (key == 0x10) {

        // Toggle to next mode
        AlleywayControlsMode = static_cast<Alleyway_Controls_Mode>(
            (static_cast<int>(AlleywayControlsMode) + 1) % static_cast<int>(Alleyway_Controls_Mode::COUNT)
            );

        switch (AlleywayControlsMode)
        {
        case Alleyway_Controls_Mode::NORMAL:
            display_message("Analogscontroller plugged out");
            break;
        case Alleyway_Controls_Mode::ANALOG_STICK:
            display_message("Analogscontroller (Analogstick Controls) plugged into linkport");
            break;
        case Alleyway_Controls_Mode::MOUSE:
            display_message("Analogscontroller (Mouse Controls) plugged into linkport");
            break;
        case Alleyway_Controls_Mode::TOUCHSCREEN:
            display_message("Analogscontroller (Touchscreen Controls) plugged into linkport");
            break;
        case Alleyway_Controls_Mode::COUNT:
            break;
        default:
            break;
        }

       

    }
  
   
}