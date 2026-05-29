#pragma once

// 1. System & Engine Includes
#include <vector>
#include <string_view>
#include <ctime>
#include "../dcgb_hotkey_target.hpp"
#include "../DoubleCherryEngine/Netplay/NetPacketManager.h"
#include "../DoubleCherryEngine/Netplay/NetplayManager.h"

// 2. Services & Core Emulation Includes
#include "../services/printer/include/image_scaler.hpp"
#include "../gb_core/gb.h"
#include "../libretro/dmy_renderer.h"

// 3. Hardware Devices & Peripherals Includes
#include "../gb_core/linkcable/include/sio_devices.hpp"
#include "../gb_core/linkcable/include/alleyway_link_controller.hpp"
#include "../gb_core/infrared/include/ir_devices.hpp"

// -----------------------------------------------------------------------------
// Libretro Subsystem & Memory Defines
// -----------------------------------------------------------------------------
#define RETRO_MEMORY_GAMEBOY_1_SRAM ((1 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_GAMEBOY_1_RTC  ((2 << 8) | RETRO_MEMORY_RTC)
#define RETRO_MEMORY_GAMEBOY_2_SRAM ((3 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_GAMEBOY_2_RTC  ((3 << 8) | RETRO_MEMORY_RTC)

#define RETRO_GAME_TYPE_GAMEBOY_LINK_2P 0x101

// -----------------------------------------------------------------------------
// Core Engine Singletons
// -----------------------------------------------------------------------------
inline NetpacketManager& netpacket_manager = NetpacketManager::getInstance();
inline NetplayManager& netplay_manager = NetplayManager::getInstance();

// -----------------------------------------------------------------------------
// Global Core Enums & States
// -----------------------------------------------------------------------------
enum mode {
    MODE_SINGLE_GAME,
    MODE_SINGLE_GAME_DUAL,
    MODE_DUAL_GAME
};

static enum mode mode = MODE_SINGLE_GAME;

// -----------------------------------------------------------------------------
// Libretro Environment Callbacks & System Information
// -----------------------------------------------------------------------------
inline retro_log_printf_t log_cb = nullptr;
inline retro_video_refresh_t video_cb = nullptr;
inline retro_audio_sample_batch_t audio_batch_cb = nullptr;
inline retro_environment_t environ_cb = nullptr;
inline retro_input_poll_t input_poll_cb = nullptr;
inline retro_input_state_t input_state_cb = nullptr;

inline uint32_t libretro_msg_interface_version = 0;
inline bool already_checked_options = false;
inline char cart_name[18];

// Fast-Forward Support Flags
static bool libretro_supports_ff_override = false;
static bool libretro_ff_enabled           = false;
static bool libretro_ff_enabled_prev      = false;
static float libretro_ff_prev_ratio       = -1.0f;

// -----------------------------------------------------------------------------
// Video, Rendering & Display Geometry Settings
// -----------------------------------------------------------------------------
static struct retro_system_av_info my_av_info;
inline bool is_gbc_rom = false;
inline bool rgb565_mode = false;
inline bool libretro_supports_persistent_buffer = false;

// Upscaling & Filters
inline int gbc_rgbSubpixel_upscale_factor = 1;
inline int gb_dotMarix_upscale_factor = 1;
inline float light_temperature = 0.0f;

// LCD Ghosting & Interlacing Effects
inline bool gbc_lcd_blur_effect_enabled = false;
inline bool useGbcLCDforDmG = false;
inline bool useDmgGhosting = true;
inline bool gbc_lcd_interlacing_enabled = false;
inline bool gbc_lcd_interfacing_fast = true;
inline float gbc_lcd_interlacing_brightness = 1.05f;

// Color Correction
inline bool gbc_color_correction_enabled = true;
inline enum color_correction_mode gbc_cc_mode = GAMBATTE_ACCURATE;

// Multi-Screen Viewport Configuration
inline bool _screen_vertical = false;
inline bool _screen_4p_split = false;
inline bool _screen_switched = false;
inline bool show_all_screens = false;
inline uint32_t _show_player_screen = 1;
inline int _number_of_local_screens = 1;

// -----------------------------------------------------------------------------
// Audio Configuration
// -----------------------------------------------------------------------------
inline bool dcgb_audio_filter_enabled = true;
inline int audio_2p_mode = 0;

// -----------------------------------------------------------------------------
// Input, Hotkeys & Hardware Sensors
// -----------------------------------------------------------------------------
static struct timespec inputpoll_start_time;
inline bool extra_inputpolling_enabled = false;
inline int extra_inputpolling_interval = 5;
inline bool libretro_supports_bitmasks = false;

// Core Hotkey Management
inline int dcgb_hotkey_pressed = -1;
inline int dcgb_last_hotkey_pressed = -1;
inline int dcgb_hotkey_frame_counter = 0;
inline I_dcgb_hotkey_target* hotkey_target = nullptr;

// Peripheral Interfaces (Rumble, LED, Sensors)
inline struct retro_rumble_interface rumble;
inline struct retro_led_interface led;
inline retro_set_rumble_state_t rumble_state_cb;
inline retro_set_led_state_t led_state_cb;

inline retro_set_sensor_state_t set_sensor_state;
inline retro_sensor_get_input_t get_sensor_input;
inline struct retro_sensor_interface sensor_interface = {
    nullptr,
    nullptr
};

// -----------------------------------------------------------------------------
// External Hardware & Accessory Emulation
// -----------------------------------------------------------------------------

// Virtual Game Boy Printer
inline bool gb_printer_png_scale_enabled = true;
inline ScaleTarget gb_printer_png_scale_mode = ScaleTarget::DIN_A4;
inline Alignment gb_printer_png_alignment = Alignment::CENTER;

// Mobile Adapter GB
inline Mobile_Adapter_GB* mobile_adapter = nullptr;
inline bool mobile_adapter_enabled = false;

// Sub-Accessories & Dynamic Controllers
inline uint32_t power_antenna_use_rumble = 0;
inline bool auto_random_tv_remote = false;
inline bool cocktail_mode_enabled = true;
inline bool cocktail_mode_vertical = true;

// Alleyway Paddle / Mouse Emulation
inline bool alleyway_analog_controller_enabled = true;
inline int16_t mouse_x = 0;
inline bool current_mouse_click = false;
Alleyway_Controls_Mode AlleywayControlsMode = Alleyway_Controls_Mode::ANALOG_STICK;

// -----------------------------------------------------------------------------
// Multiplayer & Netplay Management
// -----------------------------------------------------------------------------
inline size_t emulated_gbs = 1;
inline size_t emulated_gbs_before_playerjoined_bypress = 1;
inline uint16_t max_gbs = 16;
inline bool emulated_gbs_changed_in_options = false;
inline bool player_joined_with_joypad_press = false;

// Link Cable & Network Topologies
inline bool gblink_enable = false;
inline bool use_multi_adapter = false;
inline bool infrared_lockstep = false;
inline bool force_linkcable_over_ip_mode = false;

// Logging & Debugging
inline bool log_2p_link = false;
inline bool logging_transfers_to_file_allowed = false;
inline bool use_tetris_4p_hack = false;
inline bool detect_as_gba = false;

// Serialization / Save States Tracking
static size_t _serialize_size[16] = { 0 };
inline std::vector<I_Savestate*> v_serializable_devices;

// Core Emulated Hardware Instances
inline std::vector<gb*> v_gb;
inline std::vector<dmy_renderer*> render;
inline link_master_device* master_link = nullptr;
inline I_Linkcable_Target* linked_target_device = nullptr;

// -----------------------------------------------------------------------------
// Libretro Input Descriptor (Supports up to 16 Players)
// -----------------------------------------------------------------------------
#define DEFINE_PLAYER_PAD(port) \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "D-Pad Left" },  \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "D-Pad Up" },    \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "D-Pad Down" },  \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "D-Pad Right" }, \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "B" },           \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "A" },           \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },      \
    { port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start" }

inline struct retro_input_descriptor input_desc[] = {
    DEFINE_PLAYER_PAD(0),  DEFINE_PLAYER_PAD(1),  DEFINE_PLAYER_PAD(2),  DEFINE_PLAYER_PAD(3),
    DEFINE_PLAYER_PAD(4),  DEFINE_PLAYER_PAD(5),  DEFINE_PLAYER_PAD(6),  DEFINE_PLAYER_PAD(7),
    DEFINE_PLAYER_PAD(8),  DEFINE_PLAYER_PAD(9),  DEFINE_PLAYER_PAD(10), DEFINE_PLAYER_PAD(11),
    DEFINE_PLAYER_PAD(12), DEFINE_PLAYER_PAD(13), DEFINE_PLAYER_PAD(14), DEFINE_PLAYER_PAD(15),
    { 0 } // Sentinel terminator required by Libretro API
};