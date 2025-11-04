#include "./include/image_scaler.hpp"
#include <algorithm>
#include <cstring>
#include <cstdint>

static void calculate_target_dimensions(ScaleTarget target, int source_width, int source_height, int& out_width, int& out_height) {
    switch (target) {
    case ScaleTarget::DIN_A4:
        out_width = 2480; out_height = 3508; break; // 300 DPI
    case ScaleTarget::DIN_A5:
        out_width = 1748; out_height = 2480; break;
    case ScaleTarget::DIN_A6:
        out_width = 1240; out_height = 1748; break;
    case ScaleTarget::THERMAL_PAPER:
        out_width = 576; out_height = 960; break; // Thermopapier (z. B. 2x Höhe)
    case ScaleTarget::NONE:
        out_width = source_width; out_height = source_height; break;
    }
}

ScaledImage ImageScaler::scale(
    const uint32_t* argb_pixels, int width, int height,
    ScaleTarget target,
    Alignment alignment
) {
    int target_width = 0, target_height = 0;
    calculate_target_dimensions(target, width, height, target_width, target_height);

    // Bei NONE keine Skalierung, direkte Übergabe
    if (target == ScaleTarget::NONE) {
        std::vector<uint8_t> buffer(target_width * target_height * 3);

        // Direkte Konvertierung von ARGB zu RGB
        for (int i = 0; i < target_width * target_height; ++i) {
            uint32_t pixel = argb_pixels[i];
            buffer[i * 3 + 0] = (pixel >> 16) & 0xFF; // R
            buffer[i * 3 + 1] = (pixel >> 8) & 0xFF;  // G
            buffer[i * 3 + 2] = pixel & 0xFF;         // B
        }

        return ScaledImage{
            /*width*/ target_width,
            /*height*/ target_height,
            /*rgb_data*/ std::move(buffer)
        };
    }

    // Berechne Skalierungsfaktor (nur uniform, kein Stretching)
    float scale = std::min(
        static_cast<float>(target_width) / width,
        static_cast<float>(target_height) / height
    );

    int new_width = static_cast<int>(width * scale);
    int new_height = static_cast<int>(height * scale);

    // Zielbild anlegen
    std::vector<uint8_t> buffer(target_width * target_height * 3, 255); // Weißer Hintergrund

    // Startpunkt abhängig von Alignment
    int offset_x = (target_width - new_width) / 2;
    int offset_y = (alignment == Alignment::TOP) ? 0 : (target_height - new_height) / 2;

    // Einfaches Nearest-Neighbor-Upscaling
    for (int y = 0; y < new_height; ++y) {
        int src_y = static_cast<int>(y / scale);
        for (int x = 0; x < new_width; ++x) {
            int src_x = static_cast<int>(x / scale);
            uint32_t pixel = argb_pixels[src_y * width + src_x];

            uint8_t r = (pixel >> 16) & 0xFF;
            uint8_t g = (pixel >> 8) & 0xFF;
            uint8_t b = pixel & 0xFF;

            int dst_index = ((offset_y + y) * target_width + (offset_x + x)) * 3;
            buffer[dst_index + 0] = r;
            buffer[dst_index + 1] = g;
            buffer[dst_index + 2] = b;
        }
    }

    return ScaledImage{
        /*width*/ target_width,
        /*height*/ target_height,
        /*rgb_data*/ std::move(buffer)
    };
}