// png_printer.cpp
#include "./include/png_printer.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../Third-Party/stb_image_write.h"

bool PNGPrinter::print(const uint32_t* argb_pixels, int width, int height, ScaleTarget scale_target, Alignment alignment) {

    std::string filepath = get_timestamped_filename();
    filepath = filepath + ".png";

   ScaledImage scaled = ImageScaler::scale(argb_pixels, width, height, scale_target, alignment);

   return stbi_write_png(
       filepath.c_str(),
                scaled.width,
                scaled.height,
                3, // RGB
                scaled.rgb_data.data(),
                scaled.width * 3
            );
}
