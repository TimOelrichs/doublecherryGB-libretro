#include <libretro.h>



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
