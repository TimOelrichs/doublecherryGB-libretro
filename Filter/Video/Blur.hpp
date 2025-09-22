#pragma once

#include "../../Util/PixelUtil.hpp"

void applyBlurRGB565(uint16_t* buffer, int width, int height) {
    std::vector<uint16_t> copy(buffer, buffer + width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int rSum = 0, gSum = 0, bSum = 0, count = 0;

            for (int dx = -1; dx <= 1; dx++) {
                int nx = x + dx;
                if (nx < 0 || nx >= width) continue;

                uint8_t r, g, b;
                unpackRGB565(copy[y * width + nx], r, g, b);

                rSum += r;
                gSum += g;
                bSum += b;
                count++;
            }

            uint8_t r = rSum / count;
            uint8_t g = gSum / count;
            uint8_t b = bSum / count;

            buffer[y * width + x] = packRGB565(r, g, b);
        }
    }
}
