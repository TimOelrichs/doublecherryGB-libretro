#pragma once
#include "inline_variables.h"
#include "libretro.h"

#if defined(__PSP__)
extern "C" uint64_t sceKernelGetSystemTimeWide(void);
#elif defined(__WIIU__)
extern "C" uint64_t OSGetTime(void);
#elif defined(_WIN32)
#else
#include <sys/time.h>
#endif

#include <chrono>
#include <thread>

// Netplay (Netpacket) interface
void handlePlayerJoined();

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

    const byte* data = reinterpret_cast<const byte*>(buf);

    
    bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 1;
    if (!isMaster) {

        //byte retbuf[1] = { v_gb[0]->receive_from_linkcable(data[0]) };
        //netpacket_send(client_id, retbuf, 1);
        v_gb[0]->receive_from_linkcable(data[0]);

        return;
    }

    v_gb[0]->get_cpu()->received_netpacket_data.push(data[0]);
    v_gb[0]->get_cpu()->waiting_for_netpacket = false;
}


// Ensure we do not have too many clients for the type of connection used.
static bool netpacket_connected(unsigned short client_id) {

    if (num_clients >= max_gbs)
        return false;

    num_clients++;

    return true;
}

static void netpacket_disconnected(unsigned short client_id) {
    num_clients--;
}

const struct retro_netpacket_callback netpacket_iface = {
  netpacket_start,          // start
  netpacket_receive,        // receive
  netpacket_stop,           // stop
  netpacket_poll_receive,   // poll
  netpacket_connected,      // connected
  netpacket_disconnected,   // disconnected
  "DoubleCherryGB netpack V1.0",   // core version char* 
};

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



    //link Alleyway paddle controller
    if (!strcmp(cart_name, "ALLEY WAY"))
    {
        alleyway_link_controller* alc = new alleyway_link_controller();
        hotkey_target = alc;
        v_gb[0]->set_linked_target(alc);

        display_message("Analog Alleyway Controller plugged in");
        display_message("Press SELECT to switch between Digital and Analog controls");


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
    if (!strcmp(cart_name, "BARDIGUN"))
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


    //Default Linkcable_Hub to auto enable gb_printer if needed
    LinkCableHUB* linkHUB = new LinkCableHUB();
    v_gb[0]->set_linked_target(linkHUB);

    if (!strncmp(cart_name, "POKEMONPINB", 11) ||       //Pokemon Pinball
        (!strncmp(cart_name, "ZELDA", 5) &&             //Zelda Link's Awakening DX 
        (strncmp(cart_name, "ZELDA N", 7) ||            //but not the Oracle Games
         strncmp(cart_name, "ZELDA D", 7))
        )
        )
    {
        //v_gb[0]->set_linked_target(new gameboy_printer());

        display_message("Game supports Gameboy Printer");
        display_message("Gameboy Printer plugged in");
        return;
    }


    //Pokemon Stuff
    if (!strncmp(cart_name, "POKEMON", 7 ) || !strncmp(cart_name, "PM_CRYSTAL", 10))
    {
        
        pokebuddy_gen1* pkbuddy = new pokebuddy_gen1(v_gb);
        hotkey_target = pkbuddy;
        v_serializable_devices.push_back(pkbuddy);

        //v_gb[0]->set_linked_target(pkbuddy);
        linkHUB->set_default_link_target(pkbuddy);

        display_message("PKMBUDDY BOY plugged in");
        display_message("Check out the CABLE CLUB for weekly Distributions!");
        
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


    //Full Changer Emulation Zok Zok Heroes
    if (!strncmp(cart_name, "ZOKZOK", 6))
    {

        
        //WIP not working yet

        display_message("Game features the Full Changer Accessory");
        display_message("Full Changer Emulation enabled! ");

        full_changer* fullchanger = new full_changer(v_gb);
        hotkey_target = fullchanger;
        v_gb[0]->set_ir_master_device(fullchanger);

        return;
        
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
        

        //WIP not working yet

        /**
        display_message("Game has UBIKEY feature");
        display_message("Ubikey Unlocker is ready");
        ubikey_unlocker* ubi_unlocker = new ubikey_unlocker(v_gb);
        v_gb[0]->set_ir_target(ubi_unlocker);
        v_gb[0]->set_ir_master_device(ubi_unlocker);
        
        */
        return;
    }
   
}


void auto_link_multiplayer() {

    //set link connections
    switch (emulated_gbs)
    {
    case 1:
    {
        //TODO only if DUAL-GAME ROM
        // set interface for netpaket api (easy pokemon trading)
        environ_cb(RETRO_ENVIRONMENT_SET_NETPACKET_INTERFACE, (void*)&netpacket_iface);

        auto_config_1p_link();
        mode = MODE_SINGLE_GAME;
        break;
    }
    case 2:
    {
        // mode = MODE_DUAL_GAME;
        mode = MODE_SINGLE_GAME_DUAL;
        // for link cables and IR:
        // if (gblink_enable) {
        v_gb[0]->set_target(v_gb[1]);
        v_gb[1]->set_target(v_gb[0]);
        //}
        break;
    }
    case 3:
    {
        mode = MODE_SINGLE_GAME_DUAL;

        if (!master_link)
        {
            std::vector<gb*> _gbs;
            _gbs.insert(_gbs.begin(), std::begin(v_gb), std::begin(v_gb) + 3);
            master_link = new dmg07(_gbs);
        }
        auto_config_4p_hack();

        break;
    }

    case 4:
    {
        mode = MODE_SINGLE_GAME_DUAL;

        //if (use_multi_adapter && !master_link)
       // if (use_multi_adapter)
        {

            std::vector<gb*> _gbs;
            _gbs.insert(_gbs.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
            master_link = new dmg07(_gbs);
        }

        if (!use_multi_adapter && gblink_enable)
        {
            v_gb[0]->set_target(v_gb[1]);
            v_gb[1]->set_target(v_gb[0]);
            v_gb[2]->set_target(v_gb[3]);
            v_gb[3]->set_target(v_gb[2]);
        }
        auto_config_4p_hack();

        break;
    }
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    {
        mode = MODE_SINGLE_GAME_DUAL;

        if (!strncmp(cart_name, "FACEBALL 2000", 13))
        {

            master_link = NULL;
            linked_target_device = new faceball2000_cable(v_gb);
            v_gb[0]->set_linked_target(linked_target_device);
            display_message("RING LINK CABLE plugged in");
            break;
        }
        if (!strcmp(cart_name, "KWIRK"))
        {

            master_link = NULL;
            linked_target_device = new hack_4p_kwirk(v_gb);
            display_message("KWIRK Multiplayer Hack Adapter plugged in");
            break;
        }
        if (!strcmp(cart_name, "TETRIS"))
        {
            master_link = new hack_4p_tetris(v_gb);
            display_message("TETRIS Battle Royal Multiplayer Hack Adapter plugged in");
            break;
        }

        master_link = new dmg07x4(v_gb, emulated_gbs);
        display_message("4x FOUR PLAYER ADAPTERs are plugged in");

        break;
    }

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


#include <chrono>
bool get_monotonic_time(struct timespec* ts) {
    auto now = std::chrono::steady_clock::now();
    auto secs = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now) -
        std::chrono::time_point_cast<std::chrono::nanoseconds>(secs);

    ts->tv_sec = secs.time_since_epoch().count();
    ts->tv_nsec = ns.count();
    return true;
}



//try to avoid missed frames, see https://bsnes.org/articles/input-latency
void performExtraInputPoll() {
    struct timespec current_time;
    get_monotonic_time(&current_time);
    //clock_gettime(CLOCK_MONOTONIC, &current_time);

    long elapsed_ms = (current_time.tv_sec - inputpoll_start_time.tv_sec) * 1000 +
        (current_time.tv_nsec - inputpoll_start_time.tv_nsec) / 1000000;

    if (elapsed_ms >= extra_inputpolling_interval) {
        get_monotonic_time(&inputpoll_start_time);
        //clock_gettime(CLOCK_MONOTONIC, &inputpoll_start_time);
        input_poll_cb();
    }
}



static void update_multiplayer_geometry() {

    int base_w = 160;
    int base_h = 144;

    int screenw = base_w;
    int screenh = base_h;

    auto split_factor = [](int n) {
        if (n < 5)  return 2;
        if (n < 10) return 3;
        return 4;
        };

    if (_screen_4p_split && (_number_of_local_screens == 1 || _show_player_screen == emulated_gbs)) {
        int f = split_factor(emulated_gbs);
        screenw *= f;
        screenh *= f;
    }
    else if (emulated_gbs > 1 && _show_player_screen == 2 && _number_of_local_screens == 1) {
        if (_screen_vertical)
            screenh *= emulated_gbs;
        else
            screenw *= emulated_gbs;
    }
    else if (_number_of_local_screens > 1) {
        if (_screen_vertical)
            screenh *= _number_of_local_screens;
        else
            screenw *= _number_of_local_screens;
    }

    my_av_info->geometry.base_width = screenw;
    my_av_info->geometry.base_height = screenh;
    my_av_info->geometry.aspect_ratio = static_cast<float>(screenw) / screenh;

    already_checked_options = true;
    environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, my_av_info);

}

static void check_variables(void)
{
    libretro_msg_interface_version = 0;
    environ_cb(RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION,
        &libretro_msg_interface_version);

    struct retro_variable var;


    var.key = "dcgb_gbc_detect_as_GBA";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {

        int value = atoi(var.value);
        detect_as_gba= (bool)value;


    }

   // { "dcgb_gbc_color_correction", "GBC Color Correction; Gambatte Simple|Gambatte Accurate|Off" },
    var.key = "dcgb_gbc_lcdcolor_correction";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {

        int value = atoi(var.value);
        gbc_color_correction_enabled = (bool)value;
		gbc_cc_mode = (color_correction_mode)value;
    
    }

    //{ "dcgb_gbc_lcd_interlacing", "GBC LCD Interlacing; Off|Fast|Linear" },
    var.key = "dcgb_gbc_lcd_interlacing";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        if (!strcmp(var.value, "Off")) {
            gbc_lcd_interlacing_enabled = false;
        }
        else gbc_lcd_interlacing_enabled = true;

        if (!strcmp(var.value, "Fast"))
        {
            gbc_lcd_interlacing_enabled = true;
            gbc_lcd_interfacing_fast = true;
        }

        if (!strcmp(var.value, "Linear"))
        {
            gbc_lcd_interlacing_enabled = true;
            gbc_lcd_interfacing_fast = false;
        }
   
    }


    var.key = "dcgb_gbc_RGB_SubPixel_Simulation";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        gbc_rgbSubpixel_upscale_factor = value;

        int screenw = 160 * gbc_rgbSubpixel_upscale_factor;
        int screenh = 144 * gbc_rgbSubpixel_upscale_factor;
        my_av_info->geometry.base_width = screenw;
        my_av_info->geometry.base_height = screenh;
        my_av_info->geometry.aspect_ratio = float(screenw) / float(screenh);


        environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, my_av_info);

    }

   

    var.key = "dcgb_gb_dotmatrix_upscale";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        gb_dotMarix_upscale_factor = value;

        int screenw = 160 * gb_dotMarix_upscale_factor;
        int screenh = 144 * gb_dotMarix_upscale_factor;
        my_av_info->geometry.base_width = screenw;
        my_av_info->geometry.base_height = screenh;
        my_av_info->geometry.aspect_ratio = float(screenw) / float(screenh);


        environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, my_av_info);

    }

    var.key = "dcgb_gbc_LCD_blur";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        gbc_lcd_blur_effect_enabled = (bool)value;

    }

    var.key = "dcgb_gb_use_gbc_lcd";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        useGbcLCDforDmG = (bool)value;

    }

    
    var.key = "dcgb_gb_lcd_ghosting";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        useDmgGhosting = (bool)value;

    }



    var.key = "dcgb_light_temperature";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        light_temperature = atof(var.value);

        for (size_t i = 0; i < emulated_gbs; i++)
        {
            //v_gb[i]->refresh_pal();
        }

    }

   
    var.key = "dcgb_gbc_lcd_interlacing_brightnesss";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        gbc_lcd_interlacing_brightness = (100.0f + (float)value) / 100.0f;

    }



     var.key = "dcgb_input_polling_rate";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        extra_inputpolling_enabled = value > 60;
        if (extra_inputpolling_enabled) extra_inputpolling_interval = value == 200 ? 5 : 8;
      
    }

 
    var.key = "dcgb_gb_printer_png_upscale";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {

        if (!strcmp(var.value, "DIN A4"))
            gb_printer_png_scale_mode = ScaleTarget::DIN_A4;
        if (!strcmp(var.value, "DIN A5"))
            gb_printer_png_scale_mode = ScaleTarget::DIN_A5;
        if (!strcmp(var.value, "DIN A6"))
            gb_printer_png_scale_mode = ScaleTarget::DIN_A6;
        if (!strcmp(var.value, "Thermalpaper"))
            gb_printer_png_scale_mode = ScaleTarget::THERMAL_PAPER;
        if (!strcmp(var.value, "Off"))
            gb_printer_png_scale_mode = ScaleTarget::NONE;

    }

   
    var.key = "dcgb_gb_printer_png_alignment";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        if (!strcmp(var.value, "Center"))
            gb_printer_png_alignment = Alignment::CENTER;
        if (!strcmp(var.value, "Top"))
            gb_printer_png_alignment = Alignment::TOP;
    }

    /*
    var.key = "dcgb_alleyway_analog_enabled";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        if (!strcmp(var.value, "Off"))
            alleyway_analog_controller_enabled = false;
        else
            alleyway_analog_controller_enabled = true;
    }
    */

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


    var.key = "dcgb_audio_filter";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        int value = atoi(var.value);
        dcgb_audio_filter_enabled = (bool)value;

    }
    
    var.key = "dcgb_emulated_gameboys";
    var.value = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
    {
        //if (!already_checked_options)
        { 
            int value = atoi(var.value);
            emulated_gbs = value;
            mode = (value == 1) ? MODE_SINGLE_GAME : mode;
            emulated_gbs_changed = true;
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
            /*
            if (!strcmp(var.value, "On"))
                logging_allowed = true;
            else logging_allowed = false;
            */
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

        // check whether to show all players' screens, or player only
        var.key = "dcgb_single_screen_mp";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {

            if (!strcmp(var.value, "all players"))
                _show_player_screen = emulated_gbs;
            else {
                int player;
                if (sscanf(var.value, "player %d only", &player) == 1 && player >= 1 && player <= 16)
                    _show_player_screen = player - 1;
            }

            if (_show_player_screen != emulated_gbs) {
                audio_2p_mode = _show_player_screen;
                var.key = "dcgb_audio_output";
                var.value = "Game Boy #" +  audio_2p_mode+1;
                environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);

            }
        }
        else
            _show_player_screen = emulated_gbs;

        update_multiplayer_geometry();

        // check whether which GameBoys audio should play
        var.key = "dcgb_audio_output";
        var.value = NULL;
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
            if (_show_player_screen != emulated_gbs) {
                audio_2p_mode = _show_player_screen;
                char buf[32];
                snprintf(buf, sizeof(buf), "Game Boy #%d", audio_2p_mode + 1);
                var.key = "dcgb_audio_output";
                var.value = buf;
                environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);

            }
            else {
                int audio_player;
                if (sscanf(var.value, "Game Boy #%d", &audio_player) == 1 && audio_player >= 1 && audio_player <= 16)
                    audio_2p_mode = audio_player - 1;

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

    mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
    current_mouse_click = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);

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
        if (dcgb_hotkey_frame_counter++ >= 60) {
            dcgb_hotkey_frame_counter = 0;
            dcgb_last_hotkey_pressed = -1;
        }
  
    }

}

void check_for_new_players() {

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

            if (emulated_gbs == 2) {
                _screen_4p_split = false;
                _screen_vertical = false;
            }
            else {
                _screen_4p_split = true;
            }
            _show_player_screen = emulated_gbs;

            auto_link_multiplayer();
            display_message("Player joined");

            update_multiplayer_geometry();
        }
    }
}




void log_save_state(uint8_t* data, size_t size)
{
    if (logging_allowed)
    {
        std::string filePath = "./dmg07_savesate_log.bin";
        std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

        for (size_t i = 0; i < size; i++)
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

#include "./services/printer/include/printer_registry.hpp"
#include "./services/printer/include/png_printer.hpp"
#include "./services/printer/include/bmp_printer.hpp"

void init_printer_registry() {
    PrinterRegistry::register_printer("png", std::make_unique<PNGPrinter>());
    PrinterRegistry::register_printer("bmp", std::make_unique<BMPPrinter>());
}

void handlePlayerJoined()
{
    auto_link_multiplayer();
    if (emulated_gbs <= 2) _screen_4p_split = false;
    else _screen_4p_split = true;
    update_multiplayer_geometry();
}