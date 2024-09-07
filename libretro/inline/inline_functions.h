#pragma once
#include "libretro.h"

void set_cart_name(byte* rombuf)
{
    memcpy(cart_name, rombuf + 0x134, 16);
    cart_name[16] = '\0';
    cart_name[17] = '\0';
}


void display_message(std::string msg_str, unsigned int seconds, bool alt)
{
    seconds %= 10;

if (libretro_msg_interface_version >= 1)
{
    struct retro_message_ext msg = {
       msg_str.data(),
       seconds * 1000,
       1,
       RETRO_LOG_INFO,
       RETRO_MESSAGE_TARGET_OSD,
       alt ? RETRO_MESSAGE_TYPE_NOTIFICATION_ALT : RETRO_MESSAGE_TYPE_NOTIFICATION,
       -1
    };
    environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE_EXT, &msg);
}
else
{
    struct retro_message msg = {
        msg_str.data(),
       seconds * 60
    };
    environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
}
}
void display_message(std::string msg_str) { display_message(msg_str, 5, false); }
void display_message_alt(std::string msg_str) { display_message(msg_str, 5, true); }

void auto_config_4p_hack()
{
    if (!strcmp(cart_name, "TETRIS"))
    {
        master_link = new hack_4p_tetris(v_gb);
        display_message("TETRIS Battle Royal Multiplayer Hack Adapter plugged in");
        return;
    }
    if (!strcmp(cart_name, "KWIRK"))
    {
         
        master_link = NULL; 
        linked_target_device = new hack_4p_kwirk(v_gb);
        display_message("KWIRK Multiplayer Hack Adapter plugged in");
        return;
    }

    if (!strcmp(cart_name, "BURGER TIME"))
    {   
        master_link = new hack_4p_burger_time_deluxe(v_gb);
        display_message("Burger Time Deluxe Multiplayer Hack Adapter plugged in");
        return;
    }



    
};

void auto_config_1p_link() {

    if (!strncmp(cart_name, "POKEMON", 7 )) 
    {
        pokebuddy_gen1* pkbuddy = new pokebuddy_gen1(v_gb);
        hotkey_target = pkbuddy;
        v_serializable_devices.push_back(pkbuddy);
        v_gb[0]->set_linked_target(pkbuddy);

        display_message("PKMBUDDY BOY plugged in");
        display_message("Check out the CABLE CLUB for weekly GEN1 Distributions!");

        
        //Mytery Gift Maschine WIP - Not working yet
        
        //TODO IR ONLY  for GEN2
        /*
        pikachu_2_gs* pika2gs = new pikachu_2_gs(v_gb);
        v_gb[0]->set_ir_target(pika2gs);
        v_gb[0]->set_ir_master_device(pika2gs);
        */
        
       
        return; 
    }

  
    //TV REMOTE Emulation
    if (!strncmp(cart_name, "SUN",  3)      ||      //ROBONPON SUN
        !strncmp(cart_name, "STAR", 4)      ||      //ROBOPON STAR (BETA)
        !strncmp(cart_name, "B-MAX", 5)     ||      //Bomberman Max Blue/Red/AIN
        !strncmp(cart_name, "F.MEGA", 6)    ||      //Bomberman Daman Bakugaiden V - Final Mega Tune
        !strncmp(cart_name, "DORAEMONMEM", 11)      //Doraemon Memories - Nobita no Omoide Daibouken
        )
    {

        display_message("Game can unlock content with a TV REMOTE");
        if(auto_random_tv_remote) display_message("TV REMOTE Emulation enabled (auto random signals) ");
        else display_message("TV REMOTE EMULATION enabled (use the Numpad to send signals)");
       
        tv_remote* tvremote = new tv_remote(v_gb);
        hotkey_target = tvremote;
        v_gb[0]->set_ir_master_device(tvremote);

        return;
    }

    //link barcodeboy
    if (!strcmp(cart_name, "BATTLE SPACE") || 
        !strcmp(cart_name, "MONSTER MAKER") ||
        !strcmp(cart_name, "KATTOBI ROAD") ||
        !strcmp(cart_name, "FAMILY JOCKEY2") ||
        !strcmp(cart_name, "FAMISTA3")
        )
    {
        display_message("Game supports BARCODE BOY! BARCODE BOY plugged in");
        barcodeboy* bcb = new barcodeboy(v_gb, cart_name);
        master_link = bcb; 
        hotkey_target = bcb;
        return; 
    }

    //link barcode taisen bardigun
    if (!strcmp(cart_name, "BARDIGUN") )
    {
        display_message("Game supports BARCODE TAISEN BARDIGUN! BARDIGUN plugged in");
        barcode_taisen_bardigun* btb = new barcode_taisen_bardigun();
        hotkey_target = btb;
        v_gb[0]->set_linked_target(btb); 
        return;
    }


    //link power_antenna/bugsensor
    if (!strncmp(cart_name, "TELEFANG", 8) ||
        !strncmp(cart_name, "BUGSITE", 7)
        )
    {
        master_link = NULL;
        v_gb[0]->set_linked_target(new power_antenna());
        display_message("Game supports POWER ANTENNA/BUGSENSOR! POWER ANTENNA/BUGSENSOR plugged in");
        return; 
    }

    //Full Changer Emulation Zok Zok Heroes
    if (!strncmp(cart_name, "ZOKZOK", 6))
    {

        /*
        //WIP not working yet

        display_message("Game features the Full Changer Accessory");
        display_message("Full Changer Emulation enabled! ");

        full_changer* fullchanger = new full_changer(v_gb);
        hotkey_target = fullchanger;
        v_gb[0]->set_ir_master_device(fullchanger);

        return;
        */
    }

    //Game and Watch Gallery Unlocker
    if (!strncmp(cart_name, "G&W GALLERY2", 12))
    {
        /*

        //WIP - not working yet
        v_gb[0]->set_linked_target(new game_and_watch_gallery_unlocker(v_gb));

        display_message("This Game as unlockable Content!");
        display_message("UNLOCKER BOY plugged in");

        */

    }


    //UbiKey Games WIP
    if (!strncmp(cart_name, "LAURA", 5) ||
        !strncmp(cart_name, "CARL LEWIS", 10) ||
        !strncmp(cart_name, "FLIPPER", 7)
        )
    {
        /*

        //WIP not working yet

        display_message("Game has UBIKEY feature");
        display_message("Ubikey Unlocker is ready");
        ubikey_unlocker* ubi_unlocker = new ubikey_unlocker(v_gb);
        v_gb[0]->set_ir_target(ubi_unlocker);
        v_gb[0]->set_ir_master_device(ubi_unlocker);
        */

        return;
    }
   
}

char* read_file_to_buffer(const char* filename, size_t* file_size) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    // Allocate memory for the buffer to hold the file contents
    char* buffer = (char*)malloc(*file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read the file into the buffer
    size_t bytes_read = fread(buffer, 1, *file_size, file);

    // Close the file
    fclose(file);

    if (bytes_read != *file_size) {
        printf("Failed to read the entire file\n");
        free(buffer);
        return NULL;
    }

    return buffer;
}


void add_new_player() {
    // v_gb.push_back(new gb)
}

void check_for_new_players() {

    for (int i = v_gb.size(); i < 4; i++)
    {
        int is_start_pressed = input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START);
        if (is_start_pressed) add_new_player();
    }

}

static void check_variables(void)
{
    libretro_msg_interface_version = 0;
    environ_cb(RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION,
        &libretro_msg_interface_version);

    struct retro_variable var;

    var.key = "dcgb_power_antenna_use_rumble";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        if (!strcmp(var.value, "Off"))
            power_antenna_use_rumble = 0;
        else if (!strcmp(var.value, "Weak"))
            power_antenna_use_rumble = 1;
        else if (!strcmp(var.value, "Strong"))
            power_antenna_use_rumble = 2;
    }

    var.key = "dcgb_tv_remote";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        if (!strncmp(var.value, "use Numpad", 9))
            auto_random_tv_remote = false;
        else
            auto_random_tv_remote = true;
    }

    var.key = "dcgb_emulated_gameboys";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
       
        if (!already_checked_options) 
        { // only apply this setting on init
            if (!strcmp(var.value, "1"))
            {
                emulated_gbs = 1;
                mode = MODE_SINGLE_GAME;
            }
            else if (!strcmp(var.value, "2"))
                emulated_gbs = 2;
            else if (!strcmp(var.value, "3"))
                emulated_gbs = 3;
            else if (!strcmp(var.value, "4"))
                emulated_gbs = 4;
            else if (!strcmp(var.value, "5"))
                emulated_gbs = 5;
            else if (!strcmp(var.value, "6"))
                emulated_gbs = 6;
            else if (!strcmp(var.value, "7"))
                emulated_gbs = 7;
            else if (!strcmp(var.value, "8"))
                emulated_gbs = 8;
            else if (!strcmp(var.value, "9"))
                emulated_gbs = 9;
            else if (!strcmp(var.value, "10"))
                emulated_gbs = 10;
            else if (!strcmp(var.value, "11"))
                emulated_gbs = 11;
            else if (!strcmp(var.value, "12"))
                emulated_gbs = 12;
            else if (!strcmp(var.value, "13"))
                emulated_gbs = 13;
            else if (!strcmp(var.value, "14"))
                emulated_gbs = 14;
            else if (!strcmp(var.value, "15"))
                emulated_gbs = 15;
            else if (!strcmp(var.value, "16"))
                emulated_gbs = 16;

        }
    }

    
    var.key = "dcgb_number_of_local_screens";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {

        //if (!already_checked_options) 
        { 
            if (!strcmp(var.value, "1"))
                _number_of_local_screens = 1;

            if (!strcmp(var.value, "2"))
                _number_of_local_screens = 2;
        }
     }

    //TODO FOR 3PLAYERS
    if (emulated_gbs > 2) {
        // check 4Player Mode
        var.key = "dcgbt_gblink_device";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {

            if (!strcmp(var.value, "4-player adapter"))
            {
                use_multi_adapter = true;
              
            }
            else if (!strcmp(var.value, "2x2 - player link"))
            {
                use_multi_adapter = false;
                if(master_link)  
                master_link = NULL;

            }
     
        }
        else
            _screen_vertical = false;
    }

    // check whether screen placement is horz (side-by-side) or vert
    if (emulated_gbs > 1)
    {
        // check whether link cable mode is enabled
        var.key = "dcgb_gblink_enable";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (!already_checked_options) { // only apply this setting on init
                if (!strcmp(var.value, "disabled"))
                    gblink_enable = false;
                else if (!strcmp(var.value, "enabled"))
                    gblink_enable = true;
            }
        }
        else
            gblink_enable = false;

        // check whether link cable mode is enabled
        var.key = "dcgb_log_link";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (!strcmp(var.value, "On"))
                logging_allowed = true;
            else logging_allowed = false; 
        }


        var.key = "dcgb_screen_placement";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (!strcmp(var.value, "left-right"))
            {
                _screen_vertical = false;
                _screen_4p_split = false;
            }
            else if (!strcmp(var.value, "top-down"))
            {
                _screen_vertical = true;
                _screen_4p_split = false;
            }
            else if (!strcmp(var.value, "splitscreen"))
            {
                _screen_vertical = false;
                _screen_4p_split = true;
            }
        }
        else
            _screen_vertical = false;

        // check whether player 1 and 2's screen placements are swapped
        var.key = "dcgb_switch_screens";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (!strcmp(var.value, "normal"))
                _screen_switched = false;
            else if (!strcmp(var.value, "switched"))
                _screen_switched = true;
        }
        else
            _screen_switched = false;

        // check whether to show both players' screens, p1 only, or p2 only
        var.key = "dcgb_single_screen_mp";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            //TODO make this cleaner and shorter
            if (!strcmp(var.value, "all players"))
                _show_player_screen = emulated_gbs;
            else if (!strcmp(var.value, "player 1 only"))
                _show_player_screen = 0;
            else if (!strcmp(var.value, "player 2 only"))
                _show_player_screen = 1;
            else if (!strcmp(var.value, "player 3 only"))
                _show_player_screen = 2;
            else if (!strcmp(var.value, "player 4 only"))
                _show_player_screen = 3;
            else if (!strcmp(var.value, "player 5 only"))
                _show_player_screen = 4;
            else if (!strcmp(var.value, "player 6 only"))
                _show_player_screen = 5;
            else if (!strcmp(var.value, "player 7 only"))
                _show_player_screen = 6;
            else if (!strcmp(var.value, "player 8 only"))
                _show_player_screen = 7;
            else if (!strcmp(var.value, "player 9 only"))
                _show_player_screen = 8;
            else if (!strcmp(var.value, "player 10 only"))
                _show_player_screen = 9;
            else if (!strcmp(var.value, "player 11 only"))
                _show_player_screen = 10;
            else if (!strcmp(var.value, "player 12 only"))
                _show_player_screen = 11;
            else if (!strcmp(var.value, "player 13 only"))
                _show_player_screen = 12;
            else if (!strcmp(var.value, "player 14 only"))
                _show_player_screen = 13;
            else if (!strcmp(var.value, "player 15 only"))
                _show_player_screen = 14;
            else if (!strcmp(var.value, "player 16 only"))
                _show_player_screen = 15;

            if (_show_player_screen != emulated_gbs) {
                audio_2p_mode = _show_player_screen;
                var.key = "dcgb_audio_output";
                var.value = "Game Boy #" +  audio_2p_mode+1;
                environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);

            }
        }
        else
            _show_player_screen = emulated_gbs;


        int screenw = 160, screenh = 144;

        if (_screen_4p_split && (_number_of_local_screens == 1 || _show_player_screen == emulated_gbs)) {

            if (emulated_gbs < 5) {
                screenw *= 2;
                screenh *= 2;
            }
            else if (emulated_gbs < 10) {
                screenw *= 3;
                screenh *= 3;
            }
            else {
                screenw *= 4;
                screenh *= 4;
            }
          
        }
        else if (emulated_gbs > 1 && _show_player_screen == 2 && _number_of_local_screens == 1)
        {
            if (_screen_vertical)
                screenh *= emulated_gbs;
            else
                screenw *= emulated_gbs;
        }
        else if (_number_of_local_screens > 1)
        {
            if (_screen_vertical)
                screenh *= _number_of_local_screens;
            else
                screenw *= _number_of_local_screens;
        }


        my_av_info->geometry.base_width = screenw;
        my_av_info->geometry.base_height = screenh;
        my_av_info->geometry.aspect_ratio = float(screenw) / float(screenh);

        already_checked_options = true;
        environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, my_av_info);


        // check whether which audio should play
        var.key = "dcgb_audio_output";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (_show_player_screen != emulated_gbs) {
                audio_2p_mode = _show_player_screen;
                var.key = "dcgb_audio_output";
                var.value = "Game Boy #" + audio_2p_mode + 1;
                environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);
            }
            else {
                //TODO make this cleaner and shorter
                if (!strcmp(var.value, "Game Boy #1"))
                    audio_2p_mode = 0;
                else if (!strcmp(var.value, "Game Boy #2"))
                    audio_2p_mode = 1;
                else if (!strcmp(var.value, "Game Boy #3"))
                    audio_2p_mode = 2;
                else if (!strcmp(var.value, "Game Boy #4"))
                    audio_2p_mode = 3;
                else if (!strcmp(var.value, "Game Boy #5"))
                    audio_2p_mode = 4;
                else if (!strcmp(var.value, "Game Boy #6"))
                    audio_2p_mode = 5;
                else if (!strcmp(var.value, "Game Boy #7"))
                    audio_2p_mode = 6;
                else if (!strcmp(var.value, "Game Boy #8"))
                    audio_2p_mode = 7;
                else if (!strcmp(var.value, "Game Boy #9"))
                    audio_2p_mode = 8;
                else if (!strcmp(var.value, "Game Boy #10"))
                    audio_2p_mode = 9;
                else if (!strcmp(var.value, "Game Boy #11"))
                    audio_2p_mode = 10;
                else if (!strcmp(var.value, "Game Boy #12"))
                    audio_2p_mode = 11;
                else if (!strcmp(var.value, "Game Boy #13"))
                    audio_2p_mode = 12;
                else if (!strcmp(var.value, "Game Boy #14"))
                    audio_2p_mode = 13;
                else if (!strcmp(var.value, "Game Boy #15"))
                    audio_2p_mode = 14;
                else if (!strcmp(var.value, "Game Boy #16"))
                    audio_2p_mode = 15;
            }
        }
        else
            _screen_switched = false;

    }
    else {

        /*
        var.key = "doublecherrygb_detect_gba";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (!strcmp(var.value, "On"))
                detect_gba = true; 
        }
        */
     
        int screenw = 160, screenh = 144;

        my_av_info->geometry.base_width = screenw;
        my_av_info->geometry.base_height = screenh;
        my_av_info->geometry.aspect_ratio = float(screenw) / float(screenh);

        already_checked_options = true;
        environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, my_av_info);
    }
}

void check_special_hotkey() {

    int16_t key_state; 
    //check upper numkeys
    for (int i = 0; i < 10; i++)
    {
        key_state = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, 48 + i);
        if (key_state)
        {
            dcgb_hotkey_pressed = i;
            return; 
        }
    }
    //check numpad keys
    for (int i = 0; i < 16; i++)
    {
        key_state = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, 256 + i);
        if (key_state)
        {
            dcgb_hotkey_pressed = i < 10 ? i : 256 + i;
            return;
        }
    }

    //check NUMLOCK
    key_state = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_NUMLOCK);
    if (key_state) {
        dcgb_hotkey_pressed = 300;
        return;
    }

    //check SELECT BUTTON
    key_state = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT);
    if (key_state)
    {
        //check if also the start button is pressed
        key_state = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START);
        dcgb_hotkey_pressed = key_state ? 0x30 : 0x10;
        return; 
    }

    dcgb_hotkey_pressed = -1; 
   
}

void hotkey_handle() {

    if (hotkey_target) {
        check_special_hotkey();
        if (dcgb_hotkey_pressed >= 0 && dcgb_hotkey_pressed != dcgb_last_hotkey_pressed) {
            dcgb_last_hotkey_pressed = dcgb_hotkey_pressed;
            hotkey_target->handle_special_hotkey(dcgb_hotkey_pressed);
            return;
        }
        if (dcgb_hotkey_frame_counter++ >= 55) {
            dcgb_hotkey_frame_counter = 0;
            dcgb_last_hotkey_pressed = -1;
        }
  
    }

}



// Netplay (Netpacket) interface

void netpacket_poll_receive() {
    if (netpacket_pollrcv_fn_ptr)
        netpacket_pollrcv_fn_ptr();
}

void netpacket_send(uint16_t client_id, const void* buf, size_t len) {
    // Force all packets to be flushed ASAP, to minimize latency.
    if (netpacket_send_fn_ptr)
        netpacket_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, buf, len, client_id);
}

static void netpacket_start(uint16_t client_id, retro_netpacket_send_t send_fn, retro_netpacket_poll_receive_t poll_receive_fn) {
    netpacket_send_fn_ptr = send_fn;
    netpacket_pollrcv_fn_ptr = poll_receive_fn;
    num_clients = 0;
    my_client_id = client_id;
}

// Netplay session ends.
static void netpacket_stop() {
    netpacket_send_fn_ptr = NULL;
    netpacket_pollrcv_fn_ptr = NULL;
}

static void netpacket_receive(const void* buf, size_t len, unsigned short client_id) {

    const byte* data = (const byte*)buf;

    bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 01;
    if (!isMaster)
    {    
        byte retbuf[1] = { v_gb[0]->receive_from_linkcable(data[0])};
        
        netpacket_send(client_id, retbuf, 1 );
        return;
    }

    v_gb[0]->get_regs()->SB = data[0]; 
    v_gb[0]->get_regs()->SC &= 3;
    v_gb[0]->get_cpu()->irq(INT_SERIAL);
}


// Ensure we do not have too many clients for the type of connection used.
static bool netpacket_connected(unsigned short client_id) {
    int max_clients = 1;

    if (num_clients >= max_clients)
        return false;

    num_clients++;
    
    //test if when can change back to normal netplay
    environ_cb(RETRO_ENVIRONMENT_SET_NETPACKET_INTERFACE, NULL);
    return true;
}

static void netpacket_disconnected(unsigned short client_id) {
    num_clients--;
}


const struct retro_netpacket_callback netpacket_iface = {
  netpacket_start,          // start
  netpacket_receive,        // receive
  netpacket_stop,           // stop
  NULL,                     // poll
  netpacket_connected,      // connected
  netpacket_disconnected,   // disconnected
  "DoubleCherryGB netpack V1.0",   // core version char* 
};

void log_save_state(uint8_t* data, size_t size)
{
    if (logging_allowed)
    {
        std::string filePath = "./dmg07_savesate_log.bin";
        std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

        for (int i = 0; i < size; i++)
        {
            ofs << data[i];
        }

        ofs.close();
    }
}

void set_memory_maps() {

    unsigned sramlen = v_gb[0]->get_rom()->get_sram_size();
    const uint64_t vram = RETRO_MEMDESC_VIDEO_RAM;
    const uint64_t rom = RETRO_MEMDESC_CONST;
    const uint64_t mainram = RETRO_MEMDESC_SYSTEM_RAM;
    struct retro_memory_map mmaps;

    struct retro_memory_descriptor descs[10] =
    {
       { mainram, v_gb[0]->get_cpu()->get_ram(),     0, 0xC000,          0, 0, 0x1000, NULL},
       { mainram, v_gb[0]->get_cpu()->get_ram(),     0x1000, 0xD000,          0, 0, 0x1000, NULL },
       { mainram, v_gb[0]->get_cpu()->get_stack() ,     0, 0xFF80,          0, 0, 0x0080, NULL},
       { vram,    v_gb[0]->get_cpu()->get_vram(),         0, 0x8000,          0, 0, 0x2000, NULL},
       {       0, v_gb[0]->get_cpu()->get_oam(),       0, 0xFE00, 0, 0, 0x00A0, NULL},
       {     rom, v_gb[0]->get_rom()->get_rom(),     0, 0x0000,          0, 0, 0x4000, NULL },
       {     rom, v_gb[0]->get_rom()->get_rom(),     0x4000, 0x4000,          0, 0, 0x4000, NULL },
       {       0, v_gb[0]->get_cpu()->get_oam(),   0, 0xFF00,          0, 0, 0x0080, NULL },
       {       0, 0,                     0,      0,          0, 0,      0,    0 },
       {       0, 0,                     0,      0,          0, 0,      0,    0 }
    };



    unsigned i = 8;
    if (sramlen)
    {
        descs[i].ptr = v_gb[0]->get_mbc()->get_sram();
        descs[i].start = 0x0000;
        descs[i].select = (size_t)~0x1FFF;
        descs[i].len = sramlen;
        i++;
    }

    if (v_gb[0]->get_rom()->get_info()->cart_type > 1)
    {
        descs[i].flags = mainram;
        descs[i].ptr = v_gb[0]->get_cpu()->get_ram();
        descs[i].start = 0x00000;
        descs[i].select = 0xFFFFA000;
        descs[i].len = 0x6000;
        i++;
    }

    mmaps.descriptors = descs;
    mmaps.num_descriptors = i;
    environ_cb(RETRO_ENVIRONMENT_SET_MEMORY_MAPS, &mmaps);

    bool yes = true;
    environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS, &yes);

}
