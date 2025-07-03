#pragma once

#include <vector>
#include <cstdint>
#include <string>

enum class ScaleTarget {
    DIN_A4,
    DIN_A5,
    DIN_A6,
    THERMAL_PAPER,
    NONE
};

enum class Alignment {
    CENTER,
    TOP
};

struct ScaledImage {
    int width;
    int height;
    std::vector<uint8_t> rgb_data; // 3 bytes per pixel (RGB)
};

class ImageScaler {
public:
    static ScaledImage scale(
        const uint32_t* argb_pixels, int width, int height,
        ScaleTarget target,
        Alignment alignment
    );
};
