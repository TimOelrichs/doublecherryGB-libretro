#pragma once
#include "../dcgb_hotkey_target.hpp"

#define RETRO_MEMORY_GAMEBOY_1_SRAM ((1 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_GAMEBOY_1_RTC ((2 << 8) | RETRO_MEMORY_RTC)
#define RETRO_MEMORY_GAMEBOY_2_SRAM ((3 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_GAMEBOY_2_RTC ((3 << 8) | RETRO_MEMORY_RTC)

#define RETRO_GAME_TYPE_GAMEBOY_LINK_2P 0x101

size_t num_clients = 0;
unsigned short my_client_id = -1;
static retro_netpacket_send_t netpacket_send_fn_ptr = NULL;
static retro_netpacket_poll_receive_t netpacket_pollrcv_fn_ptr = NULL;

struct retro_rumble_interface rumble;
struct retro_led_interface led;
retro_set_rumble_state_t rumble_state_cb;
retro_set_led_state_t led_state_cb;

unsigned int power_antenna_use_rumble = 0;
bool auto_random_tv_remote; 
bool alleyway_analog_controller_enabled = true;
bool cocktail_mode_enabled = true;
bool cocktail_mode_vertical = true;

int dcgb_hotkey_pressed = -1;
int dcgb_last_hotkey_pressed = -1;
int dcgb_hotkey_frame_counter = 0;
I_dcgb_hotkey_target* hotkey_target = NULL; 

bool dcgb_audio_filter_enabled = true;
//GB Printer Vars
bool gb_printer_png_scale_enabled = true;
ScaleTarget gb_printer_png_scale_mode = ScaleTarget::DIN_A4;
Alignment gb_printer_png_alignment = Alignment::CENTER;
bool player_joined_with_joypad_press = false;

//Alleyway Analog Controller var, TODO: to set in core options 
#include "../gb_core/linkcable/include/alleyway_link_controller.hpp"

int16_t mouse_x;
bool current_mouse_click;

Alleyway_Controls_Mode AlleywayControlsMode = Alleyway_Controls_Mode::ANALOG_STICK;

// Sensor Interface
retro_set_sensor_state_t set_sensor_state = NULL;
retro_sensor_get_input_t get_sensor_input = NULL;
struct retro_sensor_interface sensor_interface = {
    set_sensor_state,
    get_sensor_input
};


static const struct retro_variable vars_single[] = {
     { "dcgb_gbc_color_correction", "GBC Color Correction; Gambatte Simple|Gambatte Accurate|Off"},
     { "dcgb_gbc_lcd_interlacing", "GBC LCD Interlacing; Off|Fast|Linear"},
     { "dcgb_gbc_lcd_interlacing_brightnesss", "GBC LCD Interlacing Brightnesss difference; 5|10|15|20|25|30|35|40|45|50"},
     { "dcgb_input_polling_rate", "Input Polling Rate (Hz); 200|120|60" },
    // { "dcgb_alleyway_analog_enabled", "emulate Alleyway Analog controls; Off|On" },
     { "dcgb_gb_printer_png_upscale", "GameBoy Printer Output Upscale; DIN A4|DIN A5|DIN A6|Thermalpaper|Off" },
     { "dcgb_gb_printer_png_alignment", "GameBoy Printer Output Allignment; Center|Top" },
    { "dcgb_emulated_gameboys", "Number of emulated Gameboys (reload); 1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16" },
    { "dcgb_tv_remote", "TV Remote Emulation; use Numpad|auto (send random signal)" },
    { "dcgb_power_antenna_use_rumble", "Power Antenna/Bugsensor use rumble; Strong|Weak|Off" },
   
    // { "doublecherrygb_detect_gba", "detect playing on gba (gba enhancements); Off|On" },
    { NULL, NULL },
};

static const struct retro_variable vars_dual[] = {
      { "dcgb_gbc_color_correction", "GBC Color Correction; Gambatte Simple|Gambatte Accurate|Off"},
       { "dcgb_input_polling_rate", "Input Polling Rate (Hz); 200|120|60" },
    { "dcgb_emulated_gameboys", "Number of emulated Gameboys (reload); 1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16" },
    { "dcgb_gblink_enable", "Link cable emulation (reload); disabled|enabled" },
    { "dcgb_screen_placement", "Screen layout; left-right|top-down" },
   // { "dcgb_switch_screens", "Switch player screens; normal|switched" },
    { "dcgb_single_screen_mp", "Show player screens; all players|player 1 only|player 2 only" },
    { "dcgb_audio_output", "Audio output; Game Boy #1|Game Boy #2" },
   // { "dcgb_log_link", "Log Link Cable Communication; Off|On" },
    { NULL, NULL },
};

static const struct retro_variable vars_tripple[] = {  
      { "dcgb_gbc_color_correction", "GBC Color Correction; Gambatte Simple|Gambatte Accurate|Off"},
      { "dcgb_input_polling_rate", "Input Polling Rate (Hz); 200|120|60" },
    { "dcgb_emulated_gameboys", "Number of emulated Gameboys (reload); 1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16" },
    { "dcgb_gblink_enable", "Link cable emulation (reload); disabled|enabled" },
     { "dcgbt_gblink_device", "Link cable device (reload); 4-player adapter" },
     { "dcgb_screen_placement", "Screen layout; left-right|splitscreen|top-down" },
    //{ "dcgb_switch_screens", "Switch player screens; normal|switched" },
    { "dcgb_single_screen_mp", "Show player screens; all players|player 1 only|player 2 only|player 3 only" },
    { "dcgb_audio_output", "Audio output; Game Boy #1|Game Boy #2|Game Boy #3" },
     //{ "dcgb_log_link", "Log Link Cable Communication; Off|On" },
    { NULL, NULL },
};

static const struct retro_variable vars_quad[] = {
      { "dcgb_gbc_color_correction", "GBC Color Correction; Gambatte Simple|Gambatte Accurate|Off"},
     { "dcgb_input_polling_rate", "Input Polling Rate (Hz); 200|120|60" },
    { "dcgb_emulated_gameboys", "Number of emulated Gameboys (reload); 1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16" },
     { "dcgb_gblink_enable", "Link cable emulation (reload); disabled|enabled" },
    { "dcgbt_gblink_device", "Link cable device (reload); 4-player adapter|2x2-player link" },
     { "dcgb_screen_placement", "Screen layout; splitscreen|top-down|left-right|" },
    { "dcgb_number_of_local_screens", "Netplay: Nummer of local players (splitscreen); 1|2" },
    { "dcgb_single_screen_mp", "Show player screens; all players|player 1 only|player 2 only|player 3 only|player 4 only|player 5 only|player 6 only|player 7 only|player 8 only|player 9 only|player 10 only|player 11 only|player 12 only|player 13 only|player 14 only|player 15 only|player 16 only" },
     { "dcgb_audio_output", "Audio output; Game Boy #1|Game Boy #2|Game Boy #3|Game Boy #4|Game Boy #5|Game Boy #6|Game Boy #7|Game Boy #8|Game Boy #9|Game Boy #10|Game Boy #11|Game Boy #12|Game Boy #13|Game Boy #14|Game Boy #15|Game Boy #16" },
   // { "dcgb_log_link", "Log Link Cable Communication; Off|On" },
    { NULL, NULL },
};

static const struct retro_subsystem_memory_info gb1_memory[] = {
    { "srm", RETRO_MEMORY_GAMEBOY_1_SRAM },
    { "rtc", RETRO_MEMORY_GAMEBOY_1_RTC },
};

static const struct retro_subsystem_memory_info gb2_memory[] = {
    { "srm", RETRO_MEMORY_GAMEBOY_2_SRAM },
    { "rtc", RETRO_MEMORY_GAMEBOY_2_RTC },
};

static const struct retro_subsystem_rom_info gb_roms[] = {

    { "GameBoy #1", "gb|gbc", false, false, false, gb1_memory, 1 },
    { "GameBoy #2", "gb|gbc", false, false, false, gb2_memory, 1 },

};

 //subsystem not working, for now disable
static const struct retro_subsystem_info subsystems[] = {
  { "2 Player Game Boy Link", "gb_link_2p", gb_roms, 2, RETRO_GAME_TYPE_GAMEBOY_LINK_2P },
  { "3 Player Game Boy with 4-Player Adapter", "gb_3p", gb_roms, 2, RETRO_GAME_TYPE_GAMEBOY_LINK_2P },
  { "4 Player Game Boy with 4-Player Adapter", "gb_4p", gb_roms, 2, RETRO_GAME_TYPE_GAMEBOY_LINK_2P },
   //{ "4 Player Game Boy 2x 2-Player Link", "gb_2x2p", gb_roms, 2, RETRO_GAME_TYPE_GAMEBOY_LINK_2P },
   { NULL },
};


enum mode {
    MODE_SINGLE_GAME,
    MODE_SINGLE_GAME_DUAL,
    MODE_DUAL_GAME
};

static enum mode mode = MODE_SINGLE_GAME;

static struct timespec inputpoll_start_time;
bool extra_inputpolling_enabled = false;
int extra_inputpolling_interval = 5;

bool is_gbc_rom = false; 
int gbc_rgbSubpixel_upscale_factor = 1;
int gb_dotMarix_upscale_factor = 1;
bool gbc_lcd_blur_effect_enabled = false;
bool gbc_color_correction_enabled = true; 
bool gbc_lcd_interlacing_enabled = false; 
bool gbc_lcd_interfacing_fast = true; 
float gbc_lcd_interlacing_brightness = 1.05f;
float light_temperature = 0.0f; 
bool useGbcLCDforDmG = false;
bool useDmgGhosting = true;
bool force_linkcable_over_ip_mode = false;
bool i_am_netplay_client = false;
bool i_am_netplay_host = false;
__uint64_t my_random_netplay_id = 0;

enum color_correction_mode gbc_cc_mode = GAMBATTE_ACCURATE;



std::vector<gb*> v_gb;
std::vector <dmy_renderer*> render;
link_master_device* master_link;
I_linkcable_target* linked_target_device; 

std::vector<I_savestate*> v_serializable_devices; 


bool use_multi_adapter = false;
bool use_tetris_4p_hack = false;
bool log_2p_link = false;
bool logging_allowed = false;
bool detect_as_gba = true;
unsigned short max_gbs = 16;


retro_log_printf_t log_cb;
retro_video_refresh_t video_cb;
retro_audio_sample_batch_t audio_batch_cb;
retro_environment_t environ_cb;
//retro_environment_t environ_cb;
retro_input_poll_t input_poll_cb;
retro_input_state_t input_state_cb;

bool _screen_vertical = false;
bool _screen_4p_split = false;
bool _screen_switched = false;
unsigned int _show_player_screen = 1;
int _number_of_local_screens = 1;
static size_t _serialize_size[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
bool show_all_screens = false;

bool gblink_enable = false;
bool emulated_gbs_changed_in_options = false;
size_t emulated_gbs = 1;
char cart_name[18];

int audio_2p_mode = 0;
// used to make certain core options only take effect once on core startup
bool already_checked_options = false;
bool libretro_supports_persistent_buffer = false;
bool libretro_supports_bitmasks = false;
//struct retro_system_av_info* my_av_info = (retro_system_av_info*)malloc(sizeof(*my_av_info));
static struct retro_system_av_info* my_av_info = nullptr;
//static struct retro_system_av_info my_av_info;

unsigned libretro_msg_interface_version = 0;

struct retro_input_descriptor input_desc[] = {
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},
        {0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},

        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},
        {2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},

        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {8, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {9, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {10, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {11, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {12, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {13, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {14, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start"},
        {15, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select"},

        {0},

};