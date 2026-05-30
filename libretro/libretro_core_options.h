#include <DoubleCherryEngine/libretro.h>

struct retro_core_option_v2_category option_cats_us[] = {

    {
        "System",
        "System",
        "Game Boy System settings"
        },
      {
	  "DotMatrixGrid",
	  "Game Boy LCD",
      "Game Boy DotMatrix Simulation settings"
      },
      {
      "LCD",
      "Game Boy Color LCD",
      "Game Boy Color LCD Simulation settings"
      },
       {
      "Input",
      "Core Inputs Settings",
      "Core Inputs Settings"
      },
{
    "Audio",
    "Core Audio Settings",
    "Core Audio Settings"
    },
      {
      "Linkcable",
      "GameBoy Linkcable Emulation",
      "Settings for different emulated Gameboy Periphererals"
      },
{
    "Infrared",
    "GameBoy Infrared Emulation",
    "Settings for different emulated Infrared feature"
    },
      {
      "Multiplayer",
      "Multiplayer",
      "Settings for 2-16 Players"
      },
{
    "Gameboy Mobile Adapter",
    "Gameboy Mobile Adapter",
    "Settings for Gameboy Mobile Adapter Emulation"
    },
{
    "Developer",
    "Developer",
    "Developer Settings"
    },

      {NULL, NULL, NULL}

};

static struct retro_core_option_v2_definition core_options_us[] = {
    //******************** SYSTEM OPTIONS *********************************
    {
        "dcgb_gbc_detect_as_GBA",
        "Detect as Played on Game Boy Advance",
        NULL,
        "Some Game Boy Color games offer enhanced features when played on a Game Boy Advance.\n\nKnown GBA-enhanced games:\n   - Shantae\n   - Wendy: Every Witch Way\n   - Zelda: Oracle of Ages / Seasons\n   - Medarot 5\n   - Mega Man Extreme 2",
        NULL,
        "System",
        {
            {"0", "Off"},
            {"1", "On"},
            {NULL, NULL}
        },
        "0",
    },
    {
        "dcgb_rtc_use_system_clock",
        "Real-Time Clock Mode",
        NULL,
        "Controls how the in-game Real-Time Clock (RTC) ticks. 'Real-Time' uses your host device's system clock (resists fast-forwarding; time passes while the emulator is closed). 'Emulated' calculates time strictly based on emulated CPU cycles.",
        NULL,
        "System",
        {
            {"0", "Emulated (Cycle-based)"},
            {"1", "Real-Time (System Clock)"},
            {NULL, NULL}
        },
        "1",
    },

    //******************** DOTMATRIX GRID OPTIONS *********************************
    {
        "dcgb_gb_lcd_ghosting",
        "Game Boy LCD Ghosting",
        NULL,
        "Simulates the screen ghosting/motion blur effect of the original Game Boy (DMG) DotMatrix LCD.",
        NULL,
        "DotMatrixGrid",
        {
            {"0", "Off"},
            {"1", "Gradient Blend"},
            {"2", "Simple Blend"},
            {NULL, NULL}
        },
        "1",
    },
    {
        "dcgb_gb_dotmatrix_upscale",
        "Game Boy DotMatrix Grid",
        NULL,
        "Simulates the physical DotMatrix pixel grid of the original Game Boy. Intended for hardware without GPU or shader support. If your device supports them, use shaders instead.",
        NULL,
        "DotMatrixGrid",
        {
            {"1", "Off"},
            {"3", "x3 (480p)"},
            {"4", "x4 (640p)"},
            {"5", "x5 (800p)"},
            {"6", "x6 (960p)"},
            {"7", "x7 (1120p)"},
            {"8", "x8 (1280p)"},
            {"9", "x9 (1440p)"},
            {NULL, NULL}
        },
        "1",
    },
    {
        "dcgb_gb_default_palette",
        "Game Boy Default Palette",
        NULL,
        "Selects the default color palette for original Game Boy games. You can cycle through palettes during gameplay by pressing L1/LB and R1/RB.",
        NULL,
        "DotMatrixGrid",
        {
            { "Black and White", NULL },
            { "Original DMG", NULL },
            { "GB Pocket", NULL },
            { "GB Light", NULL },
            { "ICE CREAM GB", NULL },
            { "AYY4", NULL },
            { "2Bit DEMICHROME", NULL },
            { "HOLLOW", NULL },
            { "Kirokaze Gameboy", NULL },
            { "Rustic GB", NULL },
            { "WISH GB", NULL },
            { "MIST GB", NULL },
            { "Crimson", NULL },
            { "Velvet Cherry GB", NULL },
            { "BLK AQU4", NULL },
            { "Nostalgia", NULL },
            { "SpaceHaze", NULL },
            { "Fiery Plague GB", NULL },
            { "Gold GB", NULL },
            { "Coldfire GB", NULL },
            { "T-Lollipop", NULL },
            { "Nymph GB", NULL },
            { "Lospec GB", NULL },
            { "Bicycle", NULL },
            { "Autumn Chill", NULL },
            { "HoneyGB", NULL },
            { "Slurry GB", NULL },
            { "RGR-Papercut4", NULL },
            { "Red Poster", NULL },
            { "minty fresh", NULL },
            { "Andrade Gameboy", NULL },
            { "Coral 4", NULL },
            { "Modern 4", NULL },
            { "bluedream4", NULL },
            { "GB MŒBIUS", NULL },
            { "GBC P005", NULL },
            { "GBC P007", NULL },
            { "GBC P008", NULL },
            { "GBC P012", NULL },
            { "GBC P016", NULL },
            { "GBC P017", NULL },
            { "GBC P01B", NULL },
            { "GBC P100", NULL },
            { "GBC P110", NULL },
            { "GBC P11C", NULL },
            { "GBC P20B", NULL },
            { "GBC P20C", NULL },
            { NULL, NULL }
        },
        "Black and White",
    },
    {
        "dcgb_gb_use_gbc_lcd",
        "Use Game Boy Color LCD Options",
        NULL,
        "Forces the core to use the Game Boy Color LCD layout and settings instead of the standard original Game Boy DotMatrix grid.",
        NULL,
        "DotMatrixGrid",
        {
            {"0", "Off"},
            {"1", "On"},
            {NULL, NULL}
        },
        "0",
    },

    //******************** LCD OPTIONS *********************************
    {
        "dcgb_gbc_lcdcolor_correction",
        "GBC Color Correction",
        NULL,
        "Simulates the screen saturation of the physical GBC display. Disable this option if you are using external RetroArch shaders, as most shaders already apply color correction.",
        NULL,
        "LCD",
        {
            {"0", "Off"},
            {"1", "Simple"},
            {"2", "Accurate"},
            {NULL, NULL}
        },
        "2",
    },
    {
        "dcgb_gbc_lcd_interlacing",
        "GBC LCD Interlacing Simulation",
        NULL,
        "Simulates the interlacing scanline effect of the Game Boy Color LCD to add visual depth.",
        NULL,
        "LCD",
        {
            {"Off", NULL},
            {"Fast", NULL},
            {"Linear", NULL},
            {NULL, NULL}
        },
        "Fast",
    },
    {
        "dcgb_gbc_lcd_interlacing_brightnesss",
        "GBC LCD Interlacing Brightness",
        NULL,
        "Adjusts the intensity of the interlacing effect. Keep this value low when not using shaders. Higher values are recommended when combined with a shader to make the effect visible.",
        NULL,
        "LCD",
        {
            {"1", NULL},
            {"2", NULL},
            {"3", NULL},
            {"4", NULL},
            {"5", NULL},
            {"10", NULL},
            {"15", NULL},
            {"20", NULL},
            {"25", NULL},
            {"30", NULL},
            {"35", NULL},
            {"40", NULL},
            {"45", NULL},
            {"50", NULL},
            {NULL, NULL}
        },
        "2",
    },
    {
        "dcgb_gbc_RGB_SubPixel_Simulation",
        "GBC LCD RGB Subpixel Scale Factor",
        NULL,
        "Simulates physical GBC subpixels by upscaling and replacing each color pixel with corresponding RGB components.\n\nFor best results:\n- Enable 'Integer Scaling' in RetroArch (Settings -> Video -> Scaling).\n- A core restart may be required after changing this setting.",
        NULL,
        "LCD",
        {
            {"1", "Off"},
            {"3", "x3 (480p)"},
            {"6", "x6 (960p)"},
            {"9", "x9 (1440p)"},
            {NULL, NULL}
        },
        "1",
    },
    {
        "dcgb_gbc_LCD_blur",
        "GBC LCD Blur",
        NULL,
        "Applies a display blur effect. This setting is highly CPU-intensive. Turn it off on weaker hardware or utilize shaders instead.",
        NULL,
        "LCD",
        {
            {"0", "Off"},
            {"1", "On"},
            {NULL, NULL}
        },
        "0",
    },
    {
        "dcgb_light_temperature",
        "Ambient Light Temperature",
        NULL,
        "Simulates an ambient light source reflecting off non-backlit Game Boy screens by adjusting the color temperature. This option has no effect when emulating an original Game Boy (DMG) model.",
        NULL,
        "LCD",
        {
            { "1.0",   "1000K (Warmest)"       },
            { "0.9",   "1550K" },
            { "0.8",   "2100K" },
            { "0.7",   "2650K" },
            { "0.6",   "3200K" },
            { "0.5",   "3750K" },
            { "0.4",   "4300K" },
            { "0.2",   "5400K" },
            { "0.1",   "5950K" },
            { "0",     "6500K (Neutral White)" },
            { "-0.1",  "7050K" },
            { "-0.2",  "7600K" },
            { "-0.3",  "8150K" },
            { "-0.4",  "8700K" },
            { "-0.5",  "9250K" },
            { "-0.6",  "9800K" },
            { "-0.7",  "10350K" },
            { "-0.8",  "10900K" },
            { "-0.9",  "11450K" },
            { "-1.0",  "12000K (Coolest)"       },
            { NULL, NULL }
        },
        "0"
    },

    //******************** AUDIO OPTIONS *********************************
    {
        "dcgb_audio_filter",
        "Audio Filter",
        NULL,
        "Enables an experimental low-pass/audio smoothing filter.",
        NULL,
        "Audio",
        {
            {"0", "Off"},
            {"1", "On"},
            {NULL, NULL}
        },
        "1",
    },

    //******************** INPUT OPTIONS *********************************
    {
        "dcgb_input_polling_rate",
        "Input Polling Rate (Hz)",
        NULL,
        "Increases the input polling rate for improved controller responsiveness. While it reduces latency in most games, it may feel overly sensitive in others.",
        NULL,
        "Input",
        {
            {"200", NULL},
            {"120", NULL},
            {"60", NULL},
            {NULL, NULL}
        },
        "200",
    },

    //******************** LINK CABLE DEVICES *********************************
    {
        "dcgb_singleplayer_linked_devive",
        "Single-Player Link Cable Device",
        NULL,
        "Keeps the link cable device on 'Auto' detection unless you need to manually force a specific hardware expansion for unsupported software.",
        NULL,
        "Linkcable",
        {
            {"auto", NULL},
            {"Off", NULL},
            {"Gameboy Printer", NULL},
            {"Barcode Boy", NULL},
            {"Bardigan Taisen Bardigun", NULL},
            {"Power Antenna/Bugsensor", NULL},
            {"PKMBuddy Boy", NULL},
            {"Alleyway Analog Controller", NULL},
            { NULL, NULL }
        },
        "auto",
    },
    {
        "dcgb_gb_printer_png_upscale",
        "Game Boy Printer Output Scale",
        NULL,
        "Sets the target paper size format for the exported Game Boy Printer image outputs.",
        NULL,
        "Linkcable",
        {
            {"Off", NULL},
            {"DIN A4", NULL},
            {"DIN A5", NULL},
            {"DIN A6", NULL},
            {"Thermalpaper", NULL},
            { NULL, NULL }
        },
        "Off",
    },
    {
        "dcgb_gb_printer_png_alignment",
        "Game Boy Printer Output Alignment",
        NULL,
        "Determines the canvas alignment for saved Game Boy Printer images.",
        NULL,
        "Linkcable",
        {
            {"Center", NULL},
            {"Top", NULL},
            { NULL, NULL }
        },
        "Center",
    },
    {
        "dcgb_pkmbuddyboy_auto_mew",
        "PKMBuddy Boy: Auto Trade Mew/Celebi",
        NULL,
        "Automatically trades a Mew or Celebi to your save file if you do not currently own one.",
        NULL,
        "Linkcable",
        {
            {"auto", NULL},
            {"Off", NULL},
            { NULL, NULL }
        },
        "auto",
    },
    {
        "dcgb_pkmbuddyboy_weekly_dtistributions events",
        "PKMBuddy Boy: Weekly Distribution Events",
        NULL,
        "Enables weekly offline simulated Pokémon distribution events via the PKMBuddy Boy emulation.",
        NULL,
        "Linkcable",
        {
            {"Off", NULL},
            {"auto", NULL},
            { NULL, NULL }
        },
        "auto",
    },
    {
        "dcgb_power_antenna_use_rumble",
        "Power Antenna / Bugsensor Rumble",
        NULL,
        "Emulates the unique hardware add-ons packaged with 'Keitai Denjuu Telefang' and 'Network Adventure Bugsite'. DoubleCherryGB replicates this functionality via the RetroArch LED API and adds rumble support.",
        NULL,
        "Linkcable",
        {
            {"Off", NULL},
            {"LED api only", NULL},
            {"Weak Rumble", NULL},
            {"Strong Rumble", NULL},
            { NULL, NULL }
        },
        "Off",
    },
    {
        "dcgb_alleyway_analog_enabled",
        "Alleyway Analog Controls",
        NULL,
        "Selects the input method used to control the Alleyway Link Cable Controller.",
        NULL,
        "Linkcable",
        {
            {"Off", NULL},
            {"left Analogstick / Steering Wheel", NULL},
            {"Mouse", NULL},
            {"Touchscreen", NULL},
            {NULL, NULL}
        },
        "Mouse",
    },

    //******************** INFRARED OPTIONS *********************************
    {
        "dcgb_infrared_lockstep",
        "Infrared Multiplayer Lockstep",
        NULL,
        "Forces synchronized step-by-step execution across core instances. Highly recommended for precise GBC Infrared (IR) features like Card Pop!. Note: Infrared emulation is experimental.",
        NULL,
        "Infrared",
        {
            {"0", "Off"},
            {"1", "On"},
            { NULL, NULL }
        },
        "0"
    },
    {
        "dcgb_infrared_tv_remote_auto_signal",
        "TV Remote - Auto Random Signal",
        NULL,
        "Certain GBC games unlock secrets via household IR TV remotes. While the keyboard Numpad can be used to send manual codes, this option periodically transmits randomized signals automatically.",
        NULL,
        "Infrared",
        {
            {"0", "Off"},
            {"1", "On"},
            { NULL, NULL }
        },
        "0"
    },

    //******************** MULTIPLAYER OPTIONS *********************************
    {
        "dcgb_emulated_gameboys",
        "Number of Emulated Game Boys",
        NULL,
        "Defines the fixed number of concurrent emulated instances (1-16). Alternatively, players can hot-join at any time during gameplay by pressing the LB/L1 button on their controller.",
        NULL,
        "Multiplayer",
        {
            {"1", NULL}, {"2", NULL}, {"3", NULL}, {"4", NULL},
            {"5", NULL}, {"6", NULL}, {"7", NULL}, {"8", NULL},
            {"9", NULL}, {"10", NULL}, {"11", NULL}, {"12", NULL},
            {"13", NULL}, {"14", NULL}, {"15", NULL}, {"16", NULL},
            {NULL, NULL}
        },
        "1",
    },
    {
        "dcgb_netplay_mode",
        "Netplay Mode",
        NULL,
        "Keep on 'Auto' unless you need to force a fallback protocol for stubborn or officially unsupported multiplayer software.",
        NULL,
        "Multiplayer",
        {
            {"0", "auto"},
            {"1", "force linkcable-over-ip / trading mode (slow)"},
            { NULL, NULL }
        },
        "0",
    },
    {
        "dcgb_multiplayer_linked_devive",
        "Multiplayer Link Adapter",
        NULL,
        "Selects the multi-device link topology. Keep on 'Auto' unless manual override is required for specific rom hacks or multiplayer games.",
        NULL,
        "Multiplayer",
        {
            {"Off", NULL},
            {"auto", NULL},
            {"2-Player Linkcable", NULL},
            {"Gameboy 4 Player Adapter", NULL},
            {"Faceball 2000 Ring-Linkcable", NULL},
            {"Tetris Multiplayer Hack Adapter", NULL},
            {"Kwirk Multiplayer Hack Adapter", NULL},
            { NULL, NULL }
        },
        "auto",
    },
    {
        "dcgb_screen_placement",
        "Local Splitscreen Layout",
        NULL,
        "Sets the screen arrangement strategy for local multiplayer sessions.",
        NULL,
        "Multiplayer",
        {
            {"grid", NULL},
            {"top-down", NULL},
            {"left-right", NULL},
            { NULL, NULL }
        },
        "grid",
    },
    {
        "dcgb_single_screen_mp",
        "Show Player Screens (Netplay)",
        NULL,
        "Configures visibility parameters for remote network players' video streams.",
        NULL,
        "Multiplayer",
        {
            {"all players", NULL},
            {"player 1 only", NULL}, {"player 2 only", NULL},
            {"player 3 only", NULL}, {"player 4 only", NULL},
            {"player 5 only", NULL}, {"player 6 only", NULL},
            {"player 7 only", NULL}, {"player 8 only", NULL},
            {"player 9 only", NULL}, {"player 10 only", NULL},
            {"player 11 only", NULL}, {"player 12 only", NULL},
            {"player 13 only", NULL}, {"player 14 only", NULL},
            {"player 15 only", NULL}, {"player 16 only", NULL},
            { NULL, NULL }
        },
        "all players",
    },
    {
        "dcgb_audio_output",
        "Multiplayer Audio Focus",
        NULL,
        "Selects which emulated Game Boy instance outputs its sound to the host system audio.",
        NULL,
        "Multiplayer",
        {
            {"Game Boy #1", NULL}, {"Game Boy #2", NULL},
            {"Game Boy #3", NULL}, {"Game Boy #4", NULL},
            {"Game Boy #5", NULL}, {"Game Boy #6", NULL},
            {"Game Boy #7", NULL}, {"Game Boy #8", NULL},
            {"Game Boy #9", NULL}, {"Game Boy #10", NULL},
            {"Game Boy #11", NULL}, {"Game Boy #12", NULL},
            {"Game Boy #13", NULL}, {"Game Boy #14", NULL},
            {"Game Boy #15", NULL}, {"Game Boy #16", NULL},
            { NULL, NULL }
        },
        "Game Boy #1",
    },
    {
        "dcgb_cocktailtable_mode",
        "Cocktail Table Display Mode",
        NULL,
        "Rotates screen configurations dynamically for usage in custom physical head-to-head Arcade Cocktail Cabinets (Strictly limited to 2-Player setups).",
        NULL,
        "Multiplayer",
        {
            {"0", "Off"},
            {"1", "vertical"},
            {"2", "horizontal"},
            { NULL, NULL }
        },
        "0",
    },

    //******************** MOBILE ADAPTER OPTIONS *********************************
    {
        "dcgb_mobile_adatper_enabled",
        "Mobile Adapter GB Emulation",
        NULL,
        "Enables emulation of the Japanese Mobile Adapter GB peripheral.\n\nSupported Titles:\n   - Pokemon Crystal (Japan or English patched)\n   - Game Boy Wars 3\n   - Hello Kitty Happy House\n   - Mobile Golf\n   - Mobile Trainer\n   - Net de Get",
        NULL,
        "Gameboy Mobile Adapter",
        {
            {"0", "Off"},
            {"1", "On"},
            {NULL, NULL}
        },
        "0",
    },
    {
        "dcgb_mobile_adatper_device_color",
        "Mobile Adapter GB Device Color",
        NULL,
        "Sets the cosmetic shell color of the emulated Mobile Adapter device.",
        NULL,
        "Gameboy Mobile Adapter",
        {
            {"8", "Blue"},
            {"9", "Yellow"},
            {"10", "Green"},
            {"11", "Red"},
            {NULL, NULL}
        },
        "8",
    },
    {
        "dcgb_mobile_adatper_dns_ip",
        "Mobile Adapter GB DNS Server IP",
        NULL,
        "Official netplay servers are offline. You can host infrastructure replacements using custom stacks like REON: https://github.com/REONTeam/reon",
        NULL,
        "Gameboy Mobile Adapter",
        {
            {"127.0.0.1", "127.0.0.1 (Localhost)"},
            {"172.18.0.1", "172.18.0.1 (Local Docker)"},
            {"3", "Custom"}
        },
        "127.0.0.1",
    },
    {
        "dcgb_mobile_adatper_dns_port",
        "Mobile Adapter GB DNS Server Port",
        NULL,
        "Specify the target port connection for the custom network infrastructure.",
        NULL,
        "Gameboy Mobile Adapter",
        {
            {"1", "53 (Default)"},
            {"2", "Custom"}
        },
        "1",
    },

    //******************** DEVELOPER OPTIONS *********************************
    {
        "dcgb_dev_log_linkcable_to_file",
        "Log Link Cable Transfers to File",
        NULL,
        "Dumps raw communication traffic data into files. Keep disabled unless actively reverse-engineering hexadecimal protocols, as heavy file I/O operations will bottleneck emulation speeds.",
        NULL,
        "Developer",
        {
            {"0", "Off"},
            {"1", "On"},
            { NULL, NULL }
        },
        "0"
    },

    { NULL, NULL, NULL, NULL, NULL }
};