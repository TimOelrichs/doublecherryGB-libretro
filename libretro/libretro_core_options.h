#include <libretro.h>



    
   
     // 
      { "dcgb_gb_printer_png_upscale", "GameBoy Printer Output Upscale; DIN A4|DIN A5|DIN A6|Thermalpaper|Off" },
      { "dcgb_gb_printer_png_alignment", "GameBoy Printer Output Allignment; Center|Top" },
     { "dcgb_emulated_gameboys", "Number of emulated Gameboys (reload); 1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16" },
     { "dcgb_tv_remote", "TV Remote Emulation; use Numpad|auto (send random signal)" },
     { "dcgb_power_antenna_use_rumble", "Power Antenna/Bugsensor use rumble; Strong|Weak|Off" },

     // { "doublecherrygb_detect_gba", "detect playing on gba (gba enhancements); Off|On" },
     { NULL, NULL },
};

static struct retro_core_option_definition core_options[] = {
   {
      "dcgb_gbc_color_correction",
      "GBC Color Correction",
      "Most Shaders already have a GBC color correction, only use this if don't use shaders.",
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
      "dcgb_gbc_lcd_interlacing",
      "GBC LCD Interlacing",
      "Simulate the interlacing of of a GBC LCD (adds more depth)",
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
      {
      "dcgb_input_polling_rate",
      "GBC Color Correction",
      "Most Shaders already have a GBC color correction, only use this if don't use shaders.",
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
   { NULL, NULL, NULL, { NULL }, NULL }
};
