#include <libretro.h>

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
      "Linkcable",
      "GameBoy Linkcable",
      "Settings for different emulated Gameboy Periphererals"
      },
      {
      "Multiplayer",
      "Multiplayer",
      "Settings for 2-16 Players"
      },

      {NULL, NULL, NULL}

};

static struct retro_core_option_v2_definition core_options_us[] = {

    //********************LCD OPTIONS START*********************************
    {
        "dcgb_gbc_detect_as_GBA",
        "detect as played on Game Boy Advance",
        NULL,
        "Some Game Boy Color Games offer GBA Enhanced features, when played on a Game Boy Advance.\n\nKnown GBA enhanced Game:\n - Shantae\n - Wendy - Every Witch Way\n - Zelda - Oracle of Ages/Seasons \n - Medarot 5\n - Mega Man Extreme 2",
        NULL,
        "System",
        {
              {"0", "Off"},
              {"1", "On"},
              {NULL, NULL}

        },
        "0",
     },
    //********************LCD OPTIONS START*********************************
   {
      "dcgb_gb_lcd_ghosting",
      "Game Boy LCD Ghosting",
      NULL,
      "Simulates the ghosting effect of the DotMatrix LCD from the Orignal Gameboy",
      NULL,
      "DotMatrixGrid",
      {
          {"0", "Off"},
          {"1", "On"},
          {NULL, NULL}

      },
      "1",
   },
     {
      "dcgb_gb_dotmatrix_upscale",
      "Game Boy DotMatrix Grid",
      NULL,
      "Simulates the DotMatrix Grid from the Orignal Gameboy.\nMeant for Hardware without GPU and Shader support. If possible, prefer shaders.",
      NULL,
      "DotMatrixGrid",
      {
          {"1", "Off"},
          {"3", "x3 (480p)"},
          {"4", "x4 (640p)"},
          {"5", "x5 (800p)"},
          {"6", "x6 (960p)"},
          {NULL, NULL}

      },
      "1",
   },
         {
      "dcgb_gb_use_gbc_lcd",
      "Use Game Boy Color LCD instead of DotMatrix",
      NULL,
      "Use Game Boy Color LCD instead of DotMatrix. Will Use the Game Boy Color LCD Settings",
      NULL,
      "DotMatrixGrid",
      {
          {"0", "Off"},
          {"1", "On"},
          {NULL, NULL}

      },
      "0",
   },
    {
     "dcgb_gbc_lcdcolor_correction",
     "GBC Color Correction",
      NULL,
      "Turn this off when you use Shaders, because most shaders already do GBC color correction.",
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
     "GBC LCD Interlacing",
      NULL,
      "Simulate the interlacing of of a GBC LCD (adds more depth)",
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
     "GBC LCD Interlacing Brightnesss difference",
      NULL,
      "Without shader leave this low, with a shader you might choose a higher value, to see the effect",
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
      "GBC LCD RGB Subpixel Simulation Scale Factor",
      NULL,
      "Simulates the the GBC RGB Subpixels, by upscaling and replacing each color Pixel with one pixel for it's RGB Value.\n\nFor best result: \n- enable Integer Scaling (Settings/Video/Scaling)\n- you might need to restart the core, after changing this setting " ,
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
      "Apply Blur Effect, this is very cpu cost-intensive. Leave to this Off, on weaker Hardware. Or try Shaders if possible" ,
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
        "Video - Ambient Light Temperature",
        "Ambient Light Temperature",
        "Simulates an ambient light's effect on non-backlit Game Boy screens, by setting a user-controlled color temperature. This option has no effect if the content is run on an original Game Boy (DMG) emulated model.",
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
            { "0.3",   "4850K" },
            { "0.2",   "5400K" },
            { "0.1",   "5950K" },
            { "0",     "6500K (Neutral White)" },
            { "-0.1",  "7050K" },
            { "-0.2",  "7600K" },
            { "-0.3",  "8150K" },
            { "-0.4",  "8700K" },
            { "-0.5",  "9250K" },
            { "-0.6",  "9800K" },
            { "-0.7", "10350K" },
            { "-0.8", "10900K" },
            { "-0.9", "11450K" },
            { "-1.0", "12000K (Coolest)"       },
            { NULL, NULL },
        },
        "0"
    },

   
    //********************LCD OPTIONS END*********************************
   
    //********************INPUT OPTIONS START*********************************
      {
     "dcgb_input_polling_rate",
     "Input Polling Rate (Hz)",
      NULL,
      "Increased Inputpolling rate for better Input reactivty. \nCan improve input reactivity for some games, but might feel to reactivate for some other games.",
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


    //********************INPUT OPTIONS END*********************************

     //********************Linkcable Devices OPTIONS Start*********************************
{
     "dcgb_singleplayer_linked_devive",
     "Singleplayer connetected Linkcabledevice",
     NULL,
     "Leave this to auto, unless you want to force a device for unsupported software",
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
     "GameBoy Printer Output Upscale",
     NULL,
     "Upscale GBPrinter Output",
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
      "Mouse",
},
{
     "dcgb_gb_printer_png_alignment",
     "GameBoy Printer Output Allignment",
     NULL,
     "GameBoy Printer Output Allignment",
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
     "PKBuddy Boy Trade Mew",
     NULL,
     "Auto Trade Mew/Celebi is you don't own one",
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
     "PKBuddy Boy Weekly offline Pokemon Distribution Events",
     NULL,
     "PKBuddy Boy Weekly Pokemon Distribution Events",
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
     "Power Antenna/Bugsensor use rumble",
     NULL,
     "These are devices, that came with the games Keitai Denjuu Telefang and Network Adventure Bugsite. DoubleCherryGB emulates this over the LED api and also adds a rumble",
     NULL,
     "Linkcable",
     {
         {"Off", NULL},
         {"LED api only", NULL},
         {"Weak Rumble", NULL},
         {"Strong Rumble", NULL},
         { NULL, NULL }

      },
      "auto",
},

  {
     "dcgb_alleyway_analog_enabled",
     "Alleyway Analog controls",
      NULL,
      "Select controlsmethod for Alleyway Linkcable Controller",
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
  //********************Linkcable Devices OPTIONS END*********************************

   //********************Multiplayer OPTIONS START*********************************

  {
     "dcgb_emulated_gameboys",
     "Number of emulated Gameboys (reload)",
      NULL,
      "Number of emulated GameBoys (1-16)",
      NULL,
      "Multiplayer",
      {
          {"1", NULL},
          {"2", NULL},
          {"3", NULL},
          {"4", NULL},
          {"5", NULL},
          {"6", NULL},
          {"7", NULL},
          {"8", NULL},
          {"9", NULL},
          {"10", NULL},
          {"11", NULL},
          {"12", NULL},
          {"13", NULL},
          {"14", NULL},
          {"15", NULL},
          {"16", NULL},
         
          {NULL, NULL}

      },
      "1",
  },
  {
     "dcgb_multiplayer_linked_devive",
     "Multiplayer",
     NULL,
     "Leave this to auto, unless you want to force a device for unsupported software",
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
     "Screen layout",
     NULL,
     "Leave this to auto, unless you want to force a device for unsupported software",
     NULL,
     "Multiplayer",
     {
         {"splitscreen", NULL},
         {"top-down", NULL},
         {"left-right", NULL},
         { NULL, NULL }

      },
      "left-right",
       },

   

 {
     "dcgb_single_screen_mp",
     "Show player screens",
     NULL,
     "Leave this to auto, unless you want to force a device for unsupported software",
     NULL,
     "Multiplayer",
     {
         {"all players", NULL},
         {"player 1 only", NULL},
         {"player 2 only", NULL},
        {"player 3 only", NULL},
        {"player 4 only", NULL},
        {"player 5 only", NULL},
        {"player 6 only", NULL},
        {"player 7 only", NULL},
        {"player 8 only", NULL},
        {"player 9 only", NULL},
        {"player 10 only", NULL},
        {"player 11 only", NULL},
        {"player 12 only", NULL},
        {"player 13 only", NULL},
        {"player 14 only", NULL},
        {"player 15 only", NULL},
        {"player 16 only", NULL},
         { NULL, NULL }

      },
      "all players",
    },
  
     {
     "dcgb_audio_output",
     "Multiplayer Audio output",
     NULL,
     "Leave this to auto, unless you want to force a device for unsupported software",
     NULL,
     "Multiplayer",
     {
         {"Game Boy #1", NULL},
         {"Game Boy #2", NULL},
        {"Game Boy #3", NULL},
        {"Game Boy #4", NULL},
        {"Game Boy #5", NULL},
        {"Game Boy #6", NULL},
        {"Game Boy #7", NULL},
        {"Game Boy #8", NULL},
        {"Game Boy #9", NULL},
        {"Game Boy #10", NULL},
        {"Game Boy #11", NULL},
        {"Game Boy #12", NULL},
        {"Game Boy #13", NULL},
        {"Game Boy #14", NULL},
        {"Game Boy #15", NULL},
        {"Game Boy #16", NULL},
         { NULL, NULL }

      },
      "Game Boy #1",
     },

    //********************Multiplayer OPTIONS END*********************************


   { NULL, NULL, NULL, NULL , NULL }
};

/*

  {
   
     
     {
        "Gambatte Simple",
        "Gambatte Accurate",
        "Off",
        NULL
     },
     "Gambatte Accurate",
     NULL,
     "LCD"
  },
   {
     "dcgb_input_polling_rate",
     "Input Polling Rate (Hz)",
     "Increased the input polling rate for better input reactivity",
     {
        "200",
        "120",
        "60",
        NULL
     },
     "200",
     NULL,
     "Input"
  },
  { "dcgb_alleyway_analog_enabled", "emulate Alleyway Analog controls; Off|On" },
        {
     "dcgb_alleyway_analog_enabled",
     "Alleyway Analog controls",
     "emulates the unreleased Alleyway Linkcable controller",
     {
        "Off",
        "Analogcontrols (right Analogstick)",
        "Analogcontrols (Mouse controls)",
        "Analogcontrols (Touchscreen)",
        NULL
     },
     "Off",
     NULL,
     "Input"
  },
  */