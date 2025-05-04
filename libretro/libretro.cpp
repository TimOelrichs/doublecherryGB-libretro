#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1 // for fopencookie hack in serialize_size
#endif

#include <stdlib.h>

#include "libretro.h"
#include "libretro_core_options.h"
#include "../gb_core/gb.h"
#include "dmy_renderer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libretro.h"

#include "../gb_core/linkcable/include/sio_devices.hpp"
#include "../gb_core/infrared/include/ir_devices.hpp"
#include "inline/inline_variables.h"
#include "inline/inline_functions.h"

static void check_variables(void);

void retro_get_system_info(struct retro_system_info *info)
{
    info->library_name = "DoubleCherryGB";
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
    info->library_version = "v0.17.0" GIT_VERSION;
    info->need_fullpath = false;
    info->valid_extensions = "gb|dmg|gbc|cgb|sgb";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{

    int w = 160, h = 144;
    info->geometry.max_width = w * max_gbs;
    info->geometry.max_height = h * max_gbs;

    if (_show_player_screen == emulated_gbs) {

        if (_screen_4p_split)
        {

            if (emulated_gbs <= 4)
            {
                h *= 2;
                w *= 2;
            }
            else if (emulated_gbs <= 9)
            {
                h *= 3;
                w *= 3;
            }
            else
            {
                h *= 4;
                w *= 4;
            }

        }
        else if (_screen_vertical)
            h *= emulated_gbs;
        else
            w *= emulated_gbs;
    }

    else if (_number_of_local_screens > 1) { 
        if (_screen_4p_split && _number_of_local_screens > 2)
        {

            if (_number_of_local_screens <= 4)
            {
                h *= 2;
                w *= 2;
            }
            else if (_number_of_local_screens <= 9)
            {
                h *= 3;
                w *= 3;
            }
            else
            {
                h *= 4;
                w *= 4;
            }

        }
        else if (_screen_vertical)
            h *= _number_of_local_screens;
        else
            w *= _number_of_local_screens;
    }
      
 
    info->timing.fps = 4194304.0 / 70224.0;
    info->timing.sample_rate = 44100.0f;
    info->geometry.base_width = w;
    info->geometry.base_height = h;
    info->geometry.aspect_ratio = float(w) / float(h);
    memcpy(my_av_info, info, sizeof(*my_av_info));
}

void retro_init(void)
{

    unsigned level = 4;
    struct retro_log_callback log;

    if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
        log_cb = log.log;
    else
        log_cb = NULL;

   
    if (environ_cb(RETRO_ENVIRONMENT_GET_LED_INTERFACE, &led)) {
        led_state_cb = led.set_led_state;
    }
    else {
        led_state_cb = 0;
    }

 
    if (environ_cb(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, &rumble)) {
        rumble_state_cb = rumble.set_rumble_state;
    }
    else {
        rumble_state_cb = 0;
    }

    environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);

    if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
        libretro_supports_bitmasks = true;

   // environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void *)vars_quad);

    check_variables();

    init_printer_registry();
}

void retro_deinit(void)
{
    libretro_supports_bitmasks = false;
}

bool retro_load_game(const struct retro_game_info *info)
{
    size_t rom_size;
    byte *rom_data;
    const struct retro_game_info_ext *info_ext = NULL;
    // environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars_single);
    // environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars_quad);
    check_variables();

    mode = MODE_SINGLE_GAME;

    unsigned i;

    
    if (!info)
        return false;

    v_gb.clear();
    render.clear();

    environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, input_desc);

    /*
    switch (emulated_gbs)
    {
    case 1:
        environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void *)vars_single);
        break;
    case 2:
        environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void *)vars_dual);
        break;
    case 3:
        environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void *)vars_tripple);
        break;
    case 4:
        environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void *)vars_quad);
        break;
    }
*/
    if (environ_cb(RETRO_ENVIRONMENT_GET_GAME_INFO_EXT, &info_ext) &&
        info_ext->persistent_data)
    {
        rom_data = (byte *)info_ext->data;
        rom_size = info_ext->size;
        libretro_supports_persistent_buffer = true;
    }
    else
    {
        rom_data = (byte *)info->data;
        rom_size = info->size;
    }

    //create gameboy instances
    for (byte i = 0; i < max_gbs; i++)
    {
        
        render.push_back(new dmy_renderer(i));
        v_gb.push_back(new gb(render[i], true, true));
        _serialize_size[i] = 0;
    }

    //load roms
    for (byte i = 0; i < max_gbs; i++)
    {
            if (!v_gb[i]->load_rom(rom_data, rom_size, NULL, 0,libretro_supports_persistent_buffer))
                return false;
       
        //v_gb[i]->set_use_gba(detect_gba);
    }

    //set cart name for autoconfig
    set_cart_name(rom_data);
    is_gbc_rom = v_gb[0]->get_rom()->get_info()->gb_type == 3;

    auto_link_multiplayer();

    check_variables();
    set_memory_maps();

    if (master_link) v_serializable_devices.push_back(master_link);

   return true;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num_info)
{

    libretro_msg_interface_version = 0;
    environ_cb(RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION,
        &libretro_msg_interface_version);

   // if (type != RETRO_GAME_TYPE_GAMEBOY_LINK_2P) return false; // all other types are unhandled for now


    // implement for 3 - 4 Player


    if (type != RETRO_GAME_TYPE_GAMEBOY_LINK_2P) {

        emulated_gbs = 2;

        //environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars_dual);
        unsigned i;

        struct retro_input_descriptor input_desc[] = {
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "Prev Audio Mode" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "Next Audio Mode" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
           { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },

           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "Prev Audio Mode" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "Next Audio Mode" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
           { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },

           { 0 },
        };

        if (!info)
            return false;

        for (i = 0; i < 2; i++)
        {
            v_gb[i] = NULL;
            render[i] = NULL;
        }

        //check_variables();
        update_multiplayer_geometry();

        environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, input_desc);

        render[0] = new dmy_renderer(0);
        v_gb[0] = new gb(render[0], true, true);
        if (!v_gb[0]->load_rom((byte*)info[0].data, info[0].size, NULL, 0, false))
            return false;

        for (i = 0; i < 2; i++)
            _serialize_size[i] = 0;

        if (gblink_enable)
        {
            render[1] = new dmy_renderer(1);
            v_gb[1] = new gb(render[1], true, true);

            if (!v_gb[1]->load_rom((byte*)info[1].data, info[1].size, NULL, 0,
                false))
                return false;

            // for link cables and IR:
            v_gb[0]->set_target(v_gb[1]);
            v_gb[1]->set_target(v_gb[0]);
        }

        mode = MODE_DUAL_GAME;
        return true;
    }


}

void retro_unload_game(void)
{
    unsigned i;
    for (i = 0; i < emulated_gbs; ++i)
    {
        if (v_gb[i])
        {
            
            v_gb[i] = NULL;
            delete render[i];
            render[i] = NULL;
        }
    }
    free(my_av_info);
    libretro_supports_persistent_buffer = false;
}

void retro_reset(void)
{
    for (int i = 0; i < emulated_gbs; ++i)
    {
        if (v_gb[i])
            v_gb[i]->reset();
    }
    /*
    if (master_link)
        master_link->reset();
        */

    for (int i = 0; i < v_serializable_devices.size(); i++)
    {
        v_serializable_devices[i]->reset();
    }
    

}

void retro_run(void)
{
    bool updated = false;

    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
        check_variables();

    get_monotonic_time(&inputpoll_start_time);
    //clock_gettime(CLOCK_MONOTONIC, &inputpoll_start_time);
    input_poll_cb();

    hotkey_handle();



    //check Multiplayer new player
    int16_t key_state;
    //check if also the start button is pressed
    if (emulated_gbs <= 16) {
        key_state = input_state_cb(emulated_gbs, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
        if (key_state)
        {
           // struct retro_variable var;
            //var.key = "dcgb_emulated_gameboys";
            //var.value = "3";
           // environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);
            ++emulated_gbs;
            check_variables();
            auto_link_multiplayer();
            display_message("Player Joined");
            update_multiplayer_geometry();
        }
    }


    for (int line = 0; line < 154; line++)
    {
        if (extra_inputpolling_enabled) performExtraInputPoll();

        for (int i = 0; i < emulated_gbs; i++)
        {
            v_gb[i]->run();
        }
        if (master_link)
            master_link->process();
    }   

}

void *retro_get_memory_data(unsigned id)
{
    switch (mode)
    {
    case MODE_SINGLE_GAME:
    case MODE_SINGLE_GAME_DUAL: /* todo: hook this properly */
    {
        switch (id)
        {
        case RETRO_MEMORY_SAVE_RAM:
            return v_gb[id]->get_rom()->get_sram();
        case RETRO_MEMORY_RTC:
            if (v_gb[id]->get_rom()->get_info()->cart_type == 0xFE)
                return &v_gb[id]->get_mbc()->huc3_baseTime;
            else
                return &(render[id]->fixed_time);
        case RETRO_MEMORY_VIDEO_RAM:
            return v_gb[id]->get_cpu()->get_vram();
        case RETRO_MEMORY_SYSTEM_RAM:
            return v_gb[id]->get_cpu()->get_ram();
        default:
            break;
        }
    }
    case MODE_DUAL_GAME:
    {
        switch (id)
        {
        case RETRO_MEMORY_GAMEBOY_1_SRAM:
            return v_gb[id]->get_rom()->get_sram();
        case RETRO_MEMORY_GAMEBOY_1_RTC:
            return &(render[id]->fixed_time);
        case RETRO_MEMORY_GAMEBOY_2_SRAM:
            return v_gb[id]->get_rom()->get_sram();
        case RETRO_MEMORY_GAMEBOY_2_RTC:
            return &(render[id]->fixed_time);
        default:
            break;
        }
    }
    }
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    switch (mode)
    {
    case MODE_SINGLE_GAME:
    case MODE_SINGLE_GAME_DUAL: /* todo: hook this properly */
    {
        switch (id)
        {
        case RETRO_MEMORY_SAVE_RAM:
            return v_gb[id]->get_rom()->get_sram_size();
        case RETRO_MEMORY_RTC:
            return sizeof(render[id]->fixed_time);
        case RETRO_MEMORY_VIDEO_RAM:
            if (v_gb[id]->get_rom()->get_info()->gb_type >= 3)
                return 0x2000 * 2; // sizeof(cpu::vram);
            return 0x2000;
        case RETRO_MEMORY_SYSTEM_RAM:
            if (v_gb[id]->get_rom()->get_info()->gb_type >= 3)
                return 0x2000 * 4; // sizeof(cpu::ram);
            return 0x2000;
        default:
            break;
        }
    }
    case MODE_DUAL_GAME:
    {
        switch (id)
        {
        case RETRO_MEMORY_GAMEBOY_1_SRAM:
            return v_gb[id]->get_rom()->get_sram_size();
        case RETRO_MEMORY_GAMEBOY_1_RTC:
            return sizeof(render[id]->fixed_time);
        case RETRO_MEMORY_GAMEBOY_2_SRAM:
            return v_gb[id]->get_rom()->get_sram_size();
        case RETRO_MEMORY_GAMEBOY_2_RTC:
            return sizeof(render[id]->fixed_time);
        default:
            break;
        }
    }
    }
    return 0;
}

// question: would saving both gb's into the same file be desirable ever?
// answer: yes, it's most likely needed to sync up netplay and for bsv records.
size_t retro_serialize_size(void)
{
    size_t _all_size = 0;

    for (int i = 0; i < max_gbs; ++i)
    {
        if (v_gb[i])
        {
            _serialize_size[i] = v_gb[i]->get_state_size();
            _all_size += _serialize_size[i];
        }
    }

    return _all_size + 0xFF00;
}

bool retro_serialize(void *data, size_t size)
{
    // if (size == retro_serialize_size())
    {
        unsigned i;
        uint8_t *ptr = (uint8_t *)data;

        for (int i = 0; i < max_gbs; ++i)
        {
            //if (v_gb[i])
            {
                v_gb[i]->save_state_mem(ptr);
                ptr += _serialize_size[i];
            }
        }
   

        for (int i = 0; i < v_serializable_devices.size(); i++)
        {
            v_serializable_devices[i]->save_state_mem(ptr);
        }

        return true;
    }
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{

    // if (size == retro_serialize_size())
    {
        unsigned i;
        uint8_t *ptr = (uint8_t *)data;

        for (i = 0; i < max_gbs; ++i)
        {
            //if (v_gb[i])
            {
                v_gb[i]->restore_state_mem(ptr);
                ptr += _serialize_size[i];
            }
        }

        for (int i = 0; i < v_serializable_devices.size(); i++)
        {
            v_serializable_devices[i]->restore_state_mem(ptr);
        }
        return true;
    }
    return false;
}

void retro_cheat_reset(void)
{
    for (int i = 0; i < emulated_gbs; ++i)
    {
        if (v_gb[i])
            v_gb[i]->get_cheat()->clear();
    }

}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
#if 1 == 0
    if (log_cb)
        log_cb(RETRO_LOG_INFO, "CHEAT:  id=%d, enabled=%d, code='%s'\n", index, enabled, code);
    // FIXME: work in progress.
    // As it stands, it seems TGB Dual only has support for Gameshark codes.
    // Unfortunately, the cheat.xml that ships with bsnes seems to only have
    // Game Genie codes, which are ROM patches rather than RAM.
    // http://nocash.emubase.de/pandocs.htm#gamegeniesharkcheats
    if (false && g_gb[0])
    {
        cheat_dat cdat;
        cheat_dat *tmp = &cdat;

        strncpy(cdat.name, code, sizeof(cdat.name));

        tmp->enable = true;
        tmp->next = NULL;

        while (false)
        { // basically, iterate over code.split('+')
            // TODO: remove all non-alnum chars here
            if (false)
            { // if strlen is 9, game genie
                // game genie format: for "ABCDEFGHI",
                // AB   = New data
                // FCDE = Memory address, XORed by 0F000h
                // GIH  = Check data (can be ignored for our purposes)
                word scramble;
                sscanf(code, "%2hhx%4hx", &tmp->dat, &scramble);
                tmp->code = 1; // TODO: test if this is correct for ROM patching
                tmp->adr = (((scramble & 0xF) << 12) ^ 0xF000) | (scramble >> 4);
            }
            else if (false)
            { // if strlen is 8, gameshark
                // gameshark format for "ABCDEFGH",
                // AB    External RAM bank number
                // CD    New Data
                // GHEF  Memory Address (internal or external RAM, A000-DFFF)
                byte adrlo, adrhi;
                sscanf(code, "%2hhx%2hhx%2hhx%2hhx", &tmp->code, &tmp->dat, &adrlo, &adrhi);
                tmp->adr = (adrhi << 8) | adrlo;
            }
            if (false)
            { // if there are more cheats left in the string
                tmp->next = new cheat_dat;
                tmp = tmp->next;
            }
        }
    }
    g_gb[0].get_cheat().add_cheat(&cdat);
#endif

    std::string code_str(code);

   // replace(code_str.begin(), code_str.end(), '+', ';');

    //if (code_str.find("-") != std::string::npos) 
    {
        v_gb[0]->set_Game_Genie(enabled, code_str);
    }

}

// start boilerplate

unsigned retro_api_version(void) { return RETRO_API_VERSION; }
unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }

void retro_set_controller_port_device(unsigned port, unsigned device) {}

void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) {}
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_set_environment(retro_environment_t cb)
{
    static const struct retro_system_content_info_override content_overrides[] = {
        {
            "gb|dmg|gbc|cgb|sgb", /* extensions */
            false,                /* need_fullpath */
            true                  /* persistent_data */
        },
        {NULL, false, false}};

    environ_cb = cb;

    /* Set core options
 * An annoyance: retro_set_environment() can be called
 * multiple times, and depending upon the current frontend
 * state various environment callbacks may be disabled.
 * This means the reported 'categories_supported' status
 * may change on subsequent iterations. We therefore have
 * to record whether 'categories_supported' is true on any
 * iteration, and latch the result */
  //  libretro_set_core_options(environ_cb, &option_categories);
    //libretro_supports_option_categories |= option_categories;

    cb(RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO, (void *)subsystems);
    /* Request a persistent content data buffer */
    cb(RETRO_ENVIRONMENT_SET_CONTENT_INFO_OVERRIDE,
       (void *)content_overrides);

    struct retro_core_options_v2 options = {
       option_cats_us,
     core_options_us // <-- v2 definitions
     
    };

    environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2, &options);
}
