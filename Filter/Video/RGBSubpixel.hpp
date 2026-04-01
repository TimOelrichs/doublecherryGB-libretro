#pragma once

#include "../../Util/PixelUtil.hpp"

#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cstdint>



const uint16_t* toSubpixelRGB6x6Bleed(
    const uint16_t* src, int width, int height,
    int bleed = 224 // 87% Basis, 13% Nachbar
) {
    int outWidth = width * 6;
    int outHeight = height * 6;
    // Framebuffer nur neu anlegen, wenn Größe sich ändert
    if ((int)frameBuffer.size() != outWidth * outHeight) {
        frameBuffer.assign(outWidth * outHeight, 0);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint16_t pixel = src[y * width + x];

            uint8_t r, g, b;
            unpackRGB565(pixel, r, g, b);

            uint16_t rPix = packRGB565(r, 0, 0);
            uint16_t gPix = packRGB565(0, g, 0);
            uint16_t bPix = packRGB565(0, 0, b);

            int dx = x * 6;
            int dy = y * 6;

            for (int yy = 0; yy < 6; yy++) {
                for (int xx = 0; xx < 6; xx++) {
                    int outIndex = (dy + yy) * outWidth + (dx + xx);

                    uint16_t col;
                    if (xx % 3 == 0) {        // R-Subpixel mit G bluten
                        col = mix565(rPix, gPix, bleed);
                    }
                    else if (xx % 3 == 1) {   // G-Subpixel mit B bluten
                        col = mix565(gPix, bPix, bleed);
                    }
                    else {                    // B-Subpixel mit R bluten
                        col = mix565(bPix, rPix, bleed);
                    }

                    frameBuffer[outIndex] = col;
                }
            }
        }
    }
    return frameBuffer.data();
}
const uint16_t* toSubpixelRGB9x9Bleed(
    const uint16_t* src, int width, int height,
    int bleed = 224 // 87% Basis, 13% Nachbar
) {
    const int scale = 9;
    int outWidth = width * scale;
    int outHeight = height * scale;

    // Framebuffer nur neu anlegen, wenn Größe sich ändert
    if ((int)frameBuffer.size() != outWidth * outHeight) {
        frameBuffer.assign(outWidth * outHeight, 0);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint16_t pixel = src[y * width + x];

            uint8_t r, g, b;
            unpackRGB565(pixel, r, g, b);

            uint16_t rPix = packRGB565(r, 0, 0);
            uint16_t gPix = packRGB565(0, g, 0);
            uint16_t bPix = packRGB565(0, 0, b);

            int dx = x * scale;
            int dy = y * scale;

            for (int yy = 0; yy < scale; yy++) {
                for (int xx = 0; xx < scale; xx++) {
                    int outIndex = (dy + yy) * outWidth + (dx + xx);

                    uint16_t col;
                    if (xx % 3 == 0) {        // R-Subpixel mit G bluten
                        col = mix565(rPix, gPix, bleed);
                    }
                    else if (xx % 3 == 1) {   // G-Subpixel mit B bluten
                        col = mix565(gPix, bPix, bleed);
                    }
                    else {                    // B-Subpixel mit R bluten
                        col = mix565(bPix, rPix, bleed);
                    }

                    frameBuffer[outIndex] = col;
                }
            }
        }
    }

    return frameBuffer.data();
}



const uint16_t* toSubpixel3x3RGB565(
    const uint16_t* src, int width, int height)
{
    int outWidth = width * 3;
    int outHeight = height * 3;
    // Framebuffer nur neu anlegen, wenn Größe sich ändert
    if ((int)frameBuffer.size() != outWidth * outHeight) {
        frameBuffer.assign(outWidth * outHeight, 0);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint16_t pixel = src[y * width + x];

            // Kanäle extrahieren
            uint8_t r = (pixel >> 11) & 0x1F; // 5 Bit
            uint8_t g = (pixel >> 5) & 0x3F; // 6 Bit
            uint8_t b = pixel & 0x1F;         // 5 Bit

            // Subpixel-Werte (nur einer aktiv je Zelle)
            uint16_t rPix = (r << 11);
            uint16_t gPix = (g << 5);
            uint16_t bPix = b;

            // Block-Koordinaten im Ziel
            int dx = x * 3;
            int dy = y * 3;

            for (int yy = 0; yy < 3; yy++) {
                int row = (dy + yy) * outWidth + dx;
                frameBuffer[row + 0] = rPix;
                frameBuffer[row + 1] = gPix;
                frameBuffer[row + 2] = bPix;
            }
        }
    }

    return frameBuffer.data();
}