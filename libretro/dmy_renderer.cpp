/*--------------------------------------------------
   TGB Dual - Gameboy Emulator -
   Copyright (C) 2001  Hii

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// libretro implementation of the renderer, should probably be renamed from dmy.

#include <string.h>
//#include <math.h>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>
#include <memory>

#include "dmy_renderer.h"
#include "../gb_core/gb.h"
#include "libretro.h"
#include "../Filter/Video/VideoFilter.hpp"
#include "gbc_lut_accurate.h"
#include "libretro.h"


extern std::vector<gb* > v_gb;
extern int emulated_gbs;
extern int max_gbs; 
extern int _number_of_local_screens;
extern bool gbc_color_correction_enabled;
extern bool is_gbc_rom; 
extern enum color_correction_mode gbc_cc_mode;
extern bool detect_as_gba;

extern bool gbc_lcd_interlacing_enabled;
extern bool gbc_lcd_interfacing_fast;
extern float gbc_lcd_interlacing_brightness;
extern float light_temperature; 
extern int gbc_rgbSubpixel_upscale_factor;
extern int gb_dotMarix_upscale_factor;
extern bool gbc_lcd_blur_effect_enabled;
extern bool useDmgGhosting;

extern retro_log_printf_t log_cb;
extern retro_video_refresh_t video_cb;
extern retro_audio_sample_batch_t audio_batch_cb;
extern retro_input_poll_t input_poll_cb;
extern retro_input_state_t input_state_cb;
extern retro_environment_t environ_cb;

extern bool gblink_enable;

extern int audio_2p_mode;

#define MSG_FRAMES 60
#define SAMPLES_PER_FRAME (44100/60)

extern bool _screen_vertical;
extern bool _screen_4p_split; 
extern bool _screen_switched; // set to draw player 2 on the left/top
extern bool libretro_supports_bitmasks;
extern int _show_player_screen; // 0 = p1 only, 1 = p2 only, 2 = both players

extern retro_set_sensor_state_t set_sensor_state;
extern retro_sensor_get_input_t get_sensor_input;
extern struct retro_sensor_interface sensor_interface;

std::array<word, GRADIENT_STEPS> blended_palette;


extern bool useGbcLCDforDmG;

static inline void temperature_tint(double temperature, double* r, double* g, double* b)
{
    if (temperature >= 0) {
        *r = 1.0;
        *g = std::pow(1.0 - temperature, 0.375);
        if (temperature >= 0.75) {
            *b = 0.0;
        }
        else {
            *b = std::sqrt(0.75 - temperature) / std::sqrt(0.75);
        }
    }
    else {
        *b = 1.0;
        double squared = temperature * temperature;
        *g = 0.125 * squared + 0.3 * temperature + 1.0;
        *r = 0.21875 * squared + 0.5 * temperature + 1.0;
    }
}



dmy_renderer::dmy_renderer(int which)
{
   which_gb = which;

   retro_pixel_format pixfmt = RETRO_PIXEL_FORMAT_RGB565;
   rgb565 = environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixfmt);

#ifndef FRONTEND_SUPPORTS_RGB565
   if (rgb565 && log_cb)
      log_cb(RETRO_LOG_INFO, "Frontend supports RGB565; will use that instead of XRGB1555.\n");
#endif

   //gradient for DMG LCD Ghosting effect
   generateGradient();
   std::fill_n(last_frame, 160 * 144, 0xFFFF);
}
word dmy_renderer::get_sensor(bool x_y) {    // Libretro Input State abfragen

    /*
    // Libretro Input State abfragen
    int16_t analog_x = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X);
    int16_t analog_y = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y);

    // Neutralwert wie im MBC7
    const uint16_t base_value = 0x81D0;

    if (x_y) {
        // X-Achse: Konvertiere -32768..32767 zu MBC7 X-Werten
        // Invertieren für korrekte Richtung
        int16_t mapped_x = -analog_x;
        return base_value + (mapped_x / 256); // Skalierung anpassen
    }
    else {
        // Y-Achse: Konvertiere -32768..32767 zu MBC7 Y-Werten  
        // INVERTIERT für vorne/hinten: Stick nach vorne = positive Werte
        int16_t mapped_y = -analog_y; // Y-Achse invertieren
        return base_value + (mapped_y / 256); // Skalierung anpassen
    }*/

    // Zuerst versuchen, Motion Controls (Gyro/Accelerometer) zu verwenden
   // Falls nicht verfügbar, auf Analogstick zurückfallen
    const uint16_t base_value = 0x81D0;
    static bool sensors_initialized = false;
    static bool sensors_available = false;

    // Sensoren beim ersten Aufruf initialisieren
    if (!sensors_initialized) {
        sensors_initialized = true;
        if (set_sensor_state) {
            // Accelerometer mit 60 Hz aktivieren
            sensors_available = set_sensor_state(0, RETRO_SENSOR_ACCELEROMETER_ENABLE, 60);
            // Gyro ebenfalls aktivieren
            set_sensor_state(0, RETRO_SENSOR_GYROSCOPE_ENABLE, 60);
        }
    }

    if (sensors_available && get_sensor_input) {
        float accel_value = 0.0f;
        unsigned sensor_id;

        if (x_y) {
            sensor_id = RETRO_SENSOR_ACCELEROMETER_X;
        }
        else {
            sensor_id = RETRO_SENSOR_ACCELEROMETER_Y;
        }

        accel_value = get_sensor_input(0, sensor_id);

        // Deadzone für kleine Bewegungen
        const float deadzone = 0.1f;
        if (std::abs(accel_value) < deadzone) {
            return base_value;
        }

        // Accelerometer-Wert skalieren (typisch ±2G)
        // 1G ≈ 0x70 Variation laut MBC7 Dokumentation
        int16_t mapped_value = static_cast<int16_t>(accel_value * 0x70);

        if (x_y) {
            // X-Achse: Neigung nach links = positive Werte
            return base_value + mapped_value;
        }
        else {
            // Y-Achse: Neigung nach vorne = positive Werte, also invertieren
            return base_value - mapped_value;
        }
    }

    // Fallback: Analogstick
    int16_t analog_x = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X);
    int16_t analog_y = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y);

    if (x_y) {
        int16_t mapped_x = -analog_x;
        return base_value + (mapped_x / 256);
    }
    else {
        int16_t mapped_y = -analog_y;
        return base_value + (mapped_y / 256);
    }
}



word dmy_renderer::map_color(word gb_col)
{
    if (is_gbc_rom && gbc_color_correction_enabled)
    {
        const unsigned r = gb_col & 0x1F;
        const unsigned g = gb_col >> 5 & 0x1F;
        const unsigned b = gb_col >> 10 & 0x1F;

        unsigned rFinal = 0;
        unsigned gFinal = 0;
        unsigned bFinal = 0;

        switch (gbc_cc_mode)
        {
            case GAMBATTE_SIMPLE:
            {
                rFinal = ((r * 13) + (g * 2) + b) >> 4;
                gFinal = ((g * 3) + b) >> 2;
                bFinal = ((r * 3) + (g * 2) + (b * 11)) >> 4;
                break;
            }
            case GAMBATTE_ACCURATE:
            {
                // Werte für GBA Enhanced Mode
                bool gba_enhanced = detect_as_gba; // eigene bool Variable
                float ccLum, ccR, ccG, ccB, ccRG, ccRB, ccGR, ccGB, ccBR, ccBG;
                float adjustedGamma;
                constexpr float CC_TARGET_GAMMA = 2.2f;
                constexpr float rgbMax = 31.0f;
                constexpr float rgbMaxInv = 1.0f / rgbMax;
                constexpr float displayGammaInv = 1.0f / CC_TARGET_GAMMA;

                if (gba_enhanced)
                {
                    ccLum = 0.94f; ccR = 0.82f; ccG = 0.665f; ccB = 0.73f;
                    ccRG = 0.125f; ccRB = 0.195f; ccGR = 0.24f; ccGB = 0.075f;
                    ccBR = -0.06f; ccBG = 0.21f;
                    adjustedGamma = CC_TARGET_GAMMA + 1.0f; // GBA_CC_GAMMA_ADJ
                }
                else
                {
                    ccLum = 0.94f; ccR = 0.82f; ccG = 0.665f; ccB = 0.73f;
                    ccRG = 0.125f; ccRB = 0.195f; ccGR = 0.24f; ccGB = 0.075f;
                    ccBR = -0.06f; ccBG = 0.21f;
                    adjustedGamma = CC_TARGET_GAMMA - 0.5f; // GBC_CC_GAMMA_ADJ
                }

                float rFloat = std::pow(static_cast<float>(r) * rgbMaxInv, adjustedGamma);
                float gFloat = std::pow(static_cast<float>(g) * rgbMaxInv, adjustedGamma);
                float bFloat = std::pow(static_cast<float>(b) * rgbMaxInv, adjustedGamma);

                float rCorrect = ccLum * ((ccR * rFloat) + (ccGR * gFloat) + (ccBR * bFloat));
                float gCorrect = ccLum * ((ccRG * rFloat) + (ccG * gFloat) + (ccBG * bFloat));
                float bCorrect = ccLum * ((ccRB * rFloat) + (ccGB * gFloat) + (ccB * bFloat));

                rCorrect = std::clamp(rCorrect, 0.0f, 1.0f);
                gCorrect = std::clamp(gCorrect, 0.0f, 1.0f);
                bCorrect = std::clamp(bCorrect, 0.0f, 1.0f);

                rCorrect = std::pow(rCorrect, displayGammaInv);
                gCorrect = std::pow(gCorrect, displayGammaInv);
                bCorrect = std::pow(bCorrect, displayGammaInv);

                rCorrect = std::clamp(rCorrect, 0.0f, 1.0f);
                gCorrect = std::clamp(gCorrect, 0.0f, 1.0f);
                bCorrect = std::clamp(bCorrect, 0.0f, 1.0f);

                rFinal = static_cast<unsigned>((rCorrect * rgbMax) + 0.5f) & 0x1F;
                gFinal = static_cast<unsigned>((gCorrect * rgbMax) + 0.5f) & 0x1F;
                bFinal = static_cast<unsigned>((bCorrect * rgbMax) + 0.5f) & 0x1F;
                break;
            }
            default:
                break;
        }

        // Licht-Temperatur anwenden
        if (light_temperature != 0.0) {
            double rf = static_cast<double>(rFinal) / 31.0;
            double gf = static_cast<double>(gFinal) / 31.0;
            double bf = static_cast<double>(bFinal) / 31.0;

            double tint_r, tint_g, tint_b;
            temperature_tint(light_temperature, &tint_r, &tint_g, &tint_b);

            rf *= tint_r; gf *= tint_g; bf *= tint_b;

            rFinal = static_cast<unsigned>(std::round(std::clamp(rf, 0.0, 1.0) * 31.0)) & 0x1F;
            gFinal = static_cast<unsigned>(std::round(std::clamp(gf, 0.0, 1.0) * 31.0)) & 0x1F;
            bFinal = static_cast<unsigned>(std::round(std::clamp(bf, 0.0, 1.0) * 31.0)) & 0x1F;
        }

        if (rgb565) return rFinal << 11 | gFinal << 6 | bFinal;
        return bFinal << 10 | gFinal << 5 | rFinal;
    }

    // Fallback:
    if (rgb565)
    {
        return ((gb_col & 0x7C00) >> 10) |   // B -> R
               ((gb_col & 0x03E0) << 1)  |   // G -> G
               ((gb_col & 0x001F) << 11);    // R -> B
    }
    else
    {
        return ((gb_col & 0x001F) << 10) |
               ((gb_col & 0x03E0))       |
               ((gb_col & 0x7C00) >> 10);
    }
}

word dmy_renderer::unmap_color(word gb_col)
{
#ifndef SKIP_COLOR_CORRECTION
#ifndef FRONTEND_SUPPORTS_RGB565
   if(rgb565)
   {
#endif
      return ((gb_col&0x001f) << 10) |
         ((gb_col&0x07c0) >>  1) |
         ((gb_col&0xf800) >> 11);
#ifndef FRONTEND_SUPPORTS_RGB565
   }
   return ((gb_col&0x001f) << 10) |
      ((gb_col&0x03e0)      ) |
      ((gb_col&0x7c00) >> 10);
#endif
#else
   return gb_col;
#endif
}

void dmy_renderer::refresh() {
   static int16_t stream[SAMPLES_PER_FRAME*2];
   // static int16_t stream[SAMPLES_PER_FRAME];

   
   //if (v_gb[1] && gblink_enable)
   //if (emulated_gbs > 1)
   if(true)
   {
        /*
      // if dual gb mode
      if (audio_2p_mode == 2)
      {
         // mix down to one per channel (dual mono)
         int16_t tmp_stream[SAMPLES_PER_FRAME*2];
         this->snd_render->render(tmp_stream, SAMPLES_PER_FRAME);
         for(int i = 0; i < SAMPLES_PER_FRAME; ++i)
         {
            int l = tmp_stream[(i*2)+0], r = tmp_stream[(i*2)+1];
            stream[(i*2)+which_gb] = int16_t( (l+r) / 2 );
         }
      }
      else if (audio_2p_mode == which_gb)
      */

       if (audio_2p_mode == which_gb)
       {
           // only play gb 0 or 1
           
           this->snd_render->render(stream, SAMPLES_PER_FRAME);
           audio_batch_cb(stream, SAMPLES_PER_FRAME);
      
           memset(stream, 0, sizeof(stream));
       
       }
       if (which_gb >= (emulated_gbs-1))
       {
         // only do audio callback after both gb's are rendered.
         //audio_batch_cb(stream, SAMPLES_PER_FRAME);

         //audio_2p_mode &= 3;
         //memset(stream, 0, sizeof(stream));
        }
   }
   else
   {
      this->snd_render->render(stream, SAMPLES_PER_FRAME);
      audio_batch_cb(stream, SAMPLES_PER_FRAME); 
   }
   fixed_time = time(NULL);

   
}

int dmy_renderer::check_pad()
{
   int16_t joypad_bits;
   if (libretro_supports_bitmasks)
      joypad_bits = input_state_cb(which_gb, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
   else
   {
      unsigned i;
      joypad_bits = 0;
      for (i = 0; i < (RETRO_DEVICE_ID_JOYPAD_R3 + 1); i++)
         joypad_bits |= input_state_cb(which_gb, RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0;
   }

   // update pad state: a,b,select,start,down,up,left,right
   return
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_A))      ? 1 : 0) << 0 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_B))      ? 1 : 0) << 1 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_X))      ? 1 : 0) << 1 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_SELECT)) ? 1 : 0) << 2 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_START))  ? 1 : 0) << 3 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_DOWN))   ? 1 : 0) << 4 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_UP))     ? 1 : 0) << 5 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT))   ? 1 : 0) << 6 |
      ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT))  ? 1 : 0) << 7;
}

void dmy_renderer::add_gbc_interlacing_effect(byte* buf, int width, int height, int pitch) {

    const float brighten_factor = gbc_lcd_interlacing_brightness;
//    int pitch = width * ((depth + 7) / 8);

    for (int y = is_odd_frame; y < height; y += 2) {


        uint16_t* line = reinterpret_cast<uint16_t*>(buf + y * pitch);


        if (gbc_lcd_interfacing_fast)
        {
            for (int x = 0; x < width; ++x) {
                uint16_t pixel = line[x];

                int r = (pixel >> 11) & 0x1F;
                int g = (pixel >> 5) & 0x3F;
                int b = pixel & 0x1F;

                r = std::min(int(r * brighten_factor), 31);
                g = std::min(int(g * brighten_factor), 63);
                b = std::min(int(b * brighten_factor), 31);

                line[x] = (r << 11) | (g << 5) | b;
            }
        }
        else {

            for (int x = 0; x < width; ++x) {
                line[x] = brighten_rgb565_hsl(line[x], (gbc_lcd_interlacing_brightness - 1.0f));
            }
        }
        
    }
    is_odd_frame = (byte)!is_odd_frame;
}


void dmy_renderer::render_screen(byte* buf, int width, int height, int depth)
{
    static byte joined_buf[160*144*2*2]; // two screens' worth of 16-bit data
    static byte joined_buf3[160 * 144 * 3 * 2]; // three screens' worth of 16-bit data
    static byte joined_buf4[160 * 144 * 4 * 2]; // four screens' worth of 16-bit data
    //static byte joined_buf8[160 * 144 * 8 * 2];
    static byte joined_buf9[160 * 144 * 9 * 2]; // three screens' worth of 16-bit data
    static byte joined_buf16[160 * 144 * 16 * 2];

    const int size_single_screen = sizeof(joined_buf) / 2;

    int pitch = width * ((depth + 7) / 8);
    int switched_gb = which_gb;
    if (_screen_switched)
        switched_gb = 1 - switched_gb;

   
    if (_number_of_local_screens == 1 || _show_player_screen == emulated_gbs)
    {
        switch (emulated_gbs)
        {
        case 1: {

			//DMG Ghosting Effect
            if (!is_gbc_rom && useDmgGhosting) {

                //DMG Ghosting Effect
                // Cast buf to 16-bit to work with 16-bit color values
                word* frame_buffer = reinterpret_cast<word*>(buf);

                for (int i = 0; i < width * height; ++i) {
                    word blended = blendPixels(last_frame[i], frame_buffer[i]);
                    last_frame[i] = frame_buffer[i]; // Update last_frame direkt
                    frame_buffer[i] = blended;        // Überschreibe buf sofort
                }
            }

			//GBC Interlacing Effect
            if ((is_gbc_rom || useGbcLCDforDmG) && gbc_lcd_interlacing_enabled)
            {
                add_gbc_interlacing_effect(buf, width, height, pitch);
            }


            //Render DMG
            if (!is_gbc_rom && !useGbcLCDforDmG)
            {

                if(gb_dotMarix_upscale_factor == 1) {
                    video_cb(buf, width, height, pitch);
                    return;
				}
 
                const uint16_t* buffer = DmgDotMatrixUpscale(reinterpret_cast<const word*>(buf), gb_dotMarix_upscale_factor);
                pitch = width * gb_dotMarix_upscale_factor * ((depth + 7) / 8);

                video_cb(buffer, width * gb_dotMarix_upscale_factor, height * gb_dotMarix_upscale_factor, pitch);
                break; 
            }


			//Render GBC
            if (gbc_rgbSubpixel_upscale_factor == 1) {
                video_cb(buf, width, height, pitch);
                return;
            }


            const uint16_t* buffer = reinterpret_cast<const word*>(buf);

            switch (gbc_rgbSubpixel_upscale_factor)
            {
            case 3:
                buffer = toSubpixel3x3RGB565(buffer, width, height);
                break;

            case 6:
                buffer = toSubpixelRGB6x6Bleed(buffer, width, height);
                break;
            case 9:
                buffer = toSubpixelRGB9x9Bleed(buffer, width, height);
                break;
            default:

                // Ensure all cases are handled, even if no specific action is required
                break;
            }

            if (gbc_lcd_blur_effect_enabled) applyBlurRGB565(const_cast<uint16_t*>(buffer), width * gbc_rgbSubpixel_upscale_factor, height * gbc_rgbSubpixel_upscale_factor);

            pitch = width * gbc_rgbSubpixel_upscale_factor * ((depth + 7) / 8);

            video_cb(buffer, width * gbc_rgbSubpixel_upscale_factor, height * gbc_rgbSubpixel_upscale_factor, pitch);

            return;
      
        }
        case 2:
        {
            // are we drawing both gb's to the screen?
            if (_show_player_screen == emulated_gbs)
            {
                if (_screen_vertical)
                {
                    memcpy(joined_buf + switched_gb * size_single_screen, buf, size_single_screen);
                    if (which_gb == 1)
                        video_cb(joined_buf, width, height * 2, pitch);
                }
                else
                {
                    for (int row = 0; row < height; ++row)
                        memcpy(joined_buf + pitch * (2 * row + switched_gb), buf + pitch * row, pitch);
                    if (which_gb == 1) {
                        //experimental GBC LCD interlacing effect
                        if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                        {
                            add_gbc_interlacing_effect(joined_buf, width*2, height, pitch * 2);
                        }
                        video_cb(joined_buf, width * 2, height, pitch * 2);
                    }
                }

            }
            else
            {
                // are we currently on the gb that we want to draw?
                // (this ignores the "switch player screens" setting)
                if (_show_player_screen == which_gb)
                    memcpy(joined_buf, buf, size_single_screen);
                if (which_gb == (emulated_gbs - 1)) {
                    //experimental GBC LCD interlacing effect
                    if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                    {
                        add_gbc_interlacing_effect(joined_buf, width, height, pitch);
                    }
                    video_cb(joined_buf, width, height, pitch);
                }
            }

            break;
        }
        case 3:
        {
            if (_show_player_screen == emulated_gbs) {
                if (_screen_4p_split)
                {
                    if (which_gb < 2) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf4 + pitch * (2 * row + switched_gb), buf + pitch * row, pitch);
                    }
                    else if (which_gb < 4) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf + pitch * (2 * row + switched_gb), buf + pitch * row, pitch);
                    }
                    if (which_gb == 2) {
                        memcpy(joined_buf4 + sizeof(joined_buf), joined_buf, sizeof(joined_buf));
                        //experimental GBC LCD interlacing effect
                        if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                        {
                            add_gbc_interlacing_effect(joined_buf4, width* 2, height* 2, pitch* 2);
                        }
                        video_cb(joined_buf4, width * 2, height * 2, pitch * 2);
                    }
                }
                else if (_screen_vertical)
                {
                    memcpy(joined_buf3 + switched_gb * size_single_screen, buf, size_single_screen);
                    if (which_gb == 2) {



                        //experimental GBC LCD interlacing effect
                        if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                        {
                            add_gbc_interlacing_effect(joined_buf3, width, height * 3, pitch);
                        }


                        video_cb(joined_buf3, width, height * 3, pitch);
                    }
                       
                }
                else
                {
                    for (int row = 0; row < height; ++row)
                        memcpy(joined_buf3 + pitch * (3 * row + switched_gb), buf + pitch * row, pitch);
                    if (which_gb == 2) {
                        //experimental GBC LCD interlacing effect
                        if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                        {
                            add_gbc_interlacing_effect(joined_buf3, width*3, height, pitch * 3);
                        }
                        video_cb(joined_buf3, width * 3, height, pitch * 3);
                    }
                       
                }
            }
            else
            {
                // are we currently on the gb that we want to draw?
                // (this ignores the "switch player screens" setting)
                if (_show_player_screen == which_gb)
                    memcpy(joined_buf, buf, size_single_screen);
                if (which_gb == emulated_gbs) {
                    //experimental GBC LCD interlacing effect
                    if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                    {
                        add_gbc_interlacing_effect(joined_buf, width, height, pitch);
                    }
                    video_cb(joined_buf, width, height, pitch);
                }
                   
            }


            break;
        }
        case 4:
        {
            if (_show_player_screen == emulated_gbs) {

                if (_screen_4p_split)
                {
                    if (which_gb < 2) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf4 + pitch * (2 * row + switched_gb), buf + pitch * row, pitch);
                    }
                    else if (which_gb < 4) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf + pitch * (2 * row + switched_gb), buf + pitch * row, pitch);
                    }
                    if (which_gb == 3) {
                        memcpy(joined_buf4 + sizeof(joined_buf), joined_buf, sizeof(joined_buf));

                        //DMG Ghosting Effect
                        if (!is_gbc_rom && useDmgGhosting) {

                            //DMG Ghosting Effect
                            // Cast buf to 16-bit to work with 16-bit color values
                            word* frame_buffer = reinterpret_cast<word*>(joined_buf4);

                            for (int i = 0; i < width * 4 * height; ++i) {
                                word blended = blendPixels(last_frame_4p_slit[i], frame_buffer[i]);
                                last_frame_4p_slit[i] = frame_buffer[i]; // Update last_frame direkt
                                frame_buffer[i] = blended;        // Überschreibe buf sofort
                            }
                        }


                        //GBC Interlacing Effect
                        if ((is_gbc_rom || useGbcLCDforDmG) && gbc_lcd_interlacing_enabled)
                        {
                            pitch = width * 2 * ((depth + 7) / 8);
                            add_gbc_interlacing_effect(joined_buf4, width * 2, height* 2, pitch);
                        }

                        //Render GBC
                        if (gbc_rgbSubpixel_upscale_factor == 1) {
                            pitch = width * 2 * ((depth + 7) / 8);
                            video_cb(joined_buf4, width * 2, height * 2, pitch);
                            return;
                        }


                        const uint16_t* buffer = reinterpret_cast<const word*>(joined_buf4);

                        switch (gbc_rgbSubpixel_upscale_factor)
                        {
                        case 3:  
                        case 6:
                            buffer = toSubpixel3x3RGB565(buffer, width * 2, height*2);
                            break;
                        default:
                            // Ensure all cases are handled, even if no specific action is required
                            break;
                        }

                        pitch = width * 3 * 2 * ((depth + 7) / 8);

                        video_cb(buffer, width * 2 * 3, height * 2 * 3, pitch);
                    }
                }

                else if (_screen_vertical)
                {
                    memcpy(joined_buf4 + switched_gb * size_single_screen, buf, size_single_screen);
                    if (which_gb == emulated_gbs - 1) {
                        if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                        {
                            add_gbc_interlacing_effect(joined_buf4, width, height * emulated_gbs, pitch);
                        }
                        video_cb(joined_buf4, width, height * emulated_gbs, pitch);
                    }
                       
                }
                else
                {
                    for (int row = 0; row < height; ++row)
                        memcpy(joined_buf4 + pitch * (emulated_gbs * row + switched_gb), buf + pitch * row, pitch);
                    if (which_gb == emulated_gbs - 1) {
                        if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                        {
                            add_gbc_interlacing_effect(joined_buf4, width* emulated_gbs, height, pitch* emulated_gbs);
                        }
                        video_cb(joined_buf4, width* emulated_gbs, height, pitch* emulated_gbs);
                    }
                       
                }
            }
            else
            {
                // are we currently on the gb that we want to draw?
                // (this ignores the "switch player screens" setting)
                if (_show_player_screen == which_gb)
                    memcpy(joined_buf, buf, size_single_screen);
                if (which_gb == (emulated_gbs - 1))
                {
                    if (is_gbc_rom && gbc_lcd_interlacing_enabled)
                    {
                        add_gbc_interlacing_effect(joined_buf, width, height, pitch);
                    }
                    video_cb(joined_buf, width, height, pitch);
                }
            }

            break;
        }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            if (_show_player_screen == emulated_gbs)
            {
                if (_screen_4p_split)
                {
                    if (which_gb < 3) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf9 + pitch * (3 * row + switched_gb), buf + pitch * row, pitch);
                    }
                    else if (which_gb < 6) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf3 + pitch * (3 * row + (switched_gb % 3)), buf + pitch * row, pitch);
                        if (which_gb == 5 || which_gb == (emulated_gbs - 1)) {
                            memcpy(joined_buf9 + (sizeof(joined_buf3)), joined_buf3, sizeof(joined_buf3));
                            memset(joined_buf3, 0, sizeof(joined_buf3));
                        }

                    }
                    else if (which_gb < 9) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf3 + pitch * (3 * row + (switched_gb % 3)), buf + pitch * row, pitch);
                        if (which_gb == (emulated_gbs - 1)) {
                            memcpy(joined_buf9 + (sizeof(joined_buf3) * 2), joined_buf3, sizeof(joined_buf3));

                        }
                    }

                    if (which_gb == (emulated_gbs - 1)) {
                        //memcpy(joined_buf16 + sizeof(joined_buf8), joined_buf8, sizeof(joined_buf8));
                        video_cb(joined_buf9, width * 3, height * 3, pitch * 3);
                    }
                }

                else if (_screen_vertical)
                {
                    memcpy(joined_buf16 + switched_gb * size_single_screen, buf, size_single_screen);
                    if (which_gb == emulated_gbs - 1)
                        video_cb(joined_buf16, width, height * emulated_gbs, pitch);
                }
                else
                {
                    for (int row = 0; row < height; ++row)
                        memcpy(joined_buf16 + pitch * (emulated_gbs * row + switched_gb), buf + pitch * row, pitch);
                    if (which_gb == emulated_gbs - 1)
                        video_cb(joined_buf16, width * emulated_gbs, height, pitch * emulated_gbs);
                }
            }
            else
            {
                // are we currently on the gb that we want to draw?
                // (this ignores the "switch player screens" setting)
                if (_show_player_screen == which_gb)
                    memcpy(joined_buf, buf, size_single_screen);
                if (which_gb == (emulated_gbs - 1))
                    video_cb(joined_buf, width, height, pitch);
            }

            break;
        }
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        {
            if (_show_player_screen == emulated_gbs) {


                if (_screen_4p_split)
                {
                    if (which_gb < 4) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf16 + pitch * (4 * row + switched_gb), buf + pitch * row, pitch);
                    }
                    else if (which_gb < 8) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf4 + pitch * (4 * row + (switched_gb % 4)), buf + pitch * row, pitch);
                        if (which_gb == 7) {
                            memcpy(joined_buf16 + (sizeof(joined_buf4)), joined_buf4, sizeof(joined_buf4));
                            memset(joined_buf4, 0, sizeof(joined_buf4));
                        }

                    }
                    else if (which_gb < 12) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf4 + pitch * (4 * row + (switched_gb % 4)), buf + pitch * row, pitch);
                        if (which_gb == 11 || which_gb == (emulated_gbs - 1)) {
                            memcpy(joined_buf16 + (sizeof(joined_buf4) * 2), joined_buf4, sizeof(joined_buf4));
                            memset(joined_buf4, 0, sizeof(joined_buf4));
                        }
                        /*
                        if (which_gb == (emulated_gbs - 1)) {
                            //memcpy(joined_buf16 + sizeof(joined_buf8), joined_buf8, sizeof(joined_buf8));
                            video_cb(joined_buf16, width * 4, height * 4, pitch * 4);
                        }*/

                    }
                    else if (which_gb < 16) {
                        for (int row = 0; row < height; ++row)
                            memcpy(joined_buf4 + pitch * (4 * row + (switched_gb % 4)), buf + pitch * row, pitch);
                        if (which_gb == (emulated_gbs - 1))
                            memcpy(joined_buf16 + (sizeof(joined_buf4) * 3), joined_buf4, sizeof(joined_buf4));
                    }
                    if (which_gb == (emulated_gbs - 1)) {
                        //memcpy(joined_buf16 + sizeof(joined_buf8), joined_buf8, sizeof(joined_buf8));
                        video_cb(joined_buf16, width * 4, height * 4, pitch * 4);
                    }
                }

                else if (_screen_vertical)
                {
                    memcpy(joined_buf16 + switched_gb * size_single_screen, buf, size_single_screen);
                    if (which_gb == emulated_gbs - 1)
                        video_cb(joined_buf16, width, height * emulated_gbs, pitch);
                }
                else
                {
                    for (int row = 0; row < height; ++row)
                        memcpy(joined_buf16 + pitch * (emulated_gbs * row + switched_gb), buf + pitch * row, pitch);
                    if (which_gb == emulated_gbs - 1)
                        video_cb(joined_buf16, width * emulated_gbs, height, pitch * emulated_gbs);
                }
            }
            else
            {
                // are we currently on the gb that we want to draw?
                // (this ignores the "switch player screens" setting)
                if (_show_player_screen == which_gb)
                    memcpy(joined_buf, buf, size_single_screen);
                if (which_gb == (emulated_gbs - 1))
                    video_cb(joined_buf, width, height, pitch);
            }


            break;
        }
        }
    }
    else 
    {
        switch (_number_of_local_screens) {
        case 2:
        {
          
            if (which_gb == _show_player_screen || which_gb == _show_player_screen+1)
            {
                if (_screen_vertical)
                {
                    memcpy(joined_buf + switched_gb * size_single_screen, buf, size_single_screen);
                    if (which_gb == _show_player_screen + 1)
                        video_cb(joined_buf, width, height * 2, pitch);
                }
                else
                {
                    for (int row = 0; row < height; ++row)
                        memcpy(joined_buf + pitch * (2 * row + switched_gb), buf + pitch * row, pitch);
                    if (which_gb == _show_player_screen + 1)
                        video_cb(joined_buf, width * 2, height, pitch * 2);
                }

            }
          
            break;
        }
        }
    }
}

byte dmy_renderer::get_time(int type)
{
   dword now = fixed_time-cur_time;

   switch(type)
   {
      case 8: // second
         return (byte)(now%60);
      case 9: // minute
         return (byte)((now/60)%60);
      case 10: // hour
         return (byte)((now/(60*60))%24);
      case 11: // day (L)
         return (byte)((now/(24*60*60))&0xff);
      case 12: // day (H)
         return (byte)((now/(256*24*60*60))&1);
   }
   return 0;
}

void dmy_renderer::set_time(int type,byte dat)
{
   dword now = fixed_time;
   dword adj = now - cur_time;

   switch(type)
   {
      case 8: // second
         adj = (adj/60)*60+(dat%60);
         break;
      case 9: // minute
         adj = (adj/(60*60))*60*60+(dat%60)*60+(adj%60);
         break;
      case 10: // hour
         adj = (adj/(24*60*60))*24*60*60+(dat%24)*60*60+(adj%(60*60));
         break;
      case 11: // day (L)
         adj = (adj/(256*24*60*60))*256*24*60*60+(dat*24*60*60)+(adj%(24*60*60));
         break;
      case 12: // day (H)
         adj = (dat&1)*256*24*60*60+(adj%(256*24*60*60));
         break;
   }
   cur_time = now - adj;
}

