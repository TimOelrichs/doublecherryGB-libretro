#pragma once

#include "../../Util/PixelUtil.hpp"

const uint16_t* DmgDotMatrixUpscale(
    const uint16_t* src, int scale_factor)
{
    int height = 144;
    int width = 160;

    int outWidth = width * scale_factor;
    int outHeight = height * scale_factor;

    if ((int)frameBuffer.size() != outWidth * outHeight) {
        frameBuffer.assign(outWidth * outHeight, 0);
    }

    constexpr uint16_t GRIDCOLOR = 0x5BEB; // #5B5D5B in RGB565

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint16_t baseColor = src[y * width + x];

            int dx = x * scale_factor;
            int dy = y * scale_factor;

            for (int yy = 0; yy < scale_factor; yy++) {
                for (int xx = 0; xx < scale_factor; xx++) {
                    uint16_t outC;

                    if (xx < (scale_factor - 1) && yy < (scale_factor - 1)) {
                        // (scale_factor-1)x(scale_factor-1) Block = Originalfarbe
                        outC = baseColor;
                    }
                    else if (xx == (scale_factor - 1) && yy == (scale_factor - 1)) {
                        // rechte untere Ecke = Gridfarbe
                        outC = GRIDCOLOR;
                    }
                    else {
                        // Rest = Blend mit Gridfarbe
                        outC = blend565(baseColor, GRIDCOLOR, 0.5f);
                    }

                    frameBuffer[(dy + yy) * outWidth + (dx + xx)] = outC;
                }
            }
        }
    }

    return frameBuffer.data();
}