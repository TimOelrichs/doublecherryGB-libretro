#pragma once

static std::vector<uint16_t> frameBuffer;

inline uint16_t packRGB565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
}

inline void unpackRGB565(uint16_t c, uint8_t& r, uint8_t& g, uint8_t& b) {
    r = (c >> 11) & 0x1F;
    g = (c >> 5) & 0x3F;
    b = c & 0x1F;
}

inline uint16_t packRGB565_from8(uint8_t r8, uint8_t g8, uint8_t b8) {
    uint16_t r5 = (uint16_t)((r8 * 31 + 127) / 255) & 0x1F;
    uint16_t g6 = (uint16_t)((g8 * 63 + 127) / 255) & 0x3F;
    uint16_t b5 = (uint16_t)((b8 * 31 + 127) / 255) & 0x1F;
    return (uint16_t)((r5 << 11) | (g6 << 5) | b5);
}



static inline uint16_t blend565(uint16_t c1, uint16_t c2, float f) {
    uint8_t r1 = ((c1 >> 11) & 0x1F) << 3;
    uint8_t g1 = ((c1 >> 5) & 0x3F) << 2;
    uint8_t b1 = (c1 & 0x1F) << 3;

    uint8_t r2 = ((c2 >> 11) & 0x1F) << 3;
    uint8_t g2 = ((c2 >> 5) & 0x3F) << 2;
    uint8_t b2 = (c2 & 0x1F) << 3;

    uint8_t r = static_cast<uint8_t>(r1 * (1.0f - f) + r2 * f);
    uint8_t g = static_cast<uint8_t>(g1 * (1.0f - f) + g2 * f);
    uint8_t b = static_cast<uint8_t>(b1 * (1.0f - f) + b2 * f);

    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}


inline uint16_t mix565(uint16_t a, uint16_t b, int factor /* 0..255 */) {
    // factor = Anteil der ersten Farbe a (z. B. 224 = 87% a, 13% b)

    int r1 = (a >> 11) & 0x1F;
    int g1 = (a >> 5) & 0x3F;
    int b1 = a & 0x1F;

    int r2 = (b >> 11) & 0x1F;
    int g2 = (b >> 5) & 0x3F;
    int b2 = b & 0x1F;

    int r = (r1 * factor + r2 * (255 - factor)) >> 8;
    int g = (g1 * factor + g2 * (255 - factor)) >> 8;
    int bC = (b1 * factor + b2 * (255 - factor)) >> 8;

    return (r << 11) | (g << 5) | bC;
}


inline int colorDistSq(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2) {
    int dr = (int)r1 - (int)r2;
    int dg = (int)g1 - (int)g2;
    int db = (int)b1 - (int)b2;
    return dr * dr + dg * dg + db * db;
}


inline uint16_t darken565(uint16_t c, int factor /*0-256*/) {
    uint8_t r, g, b;
    unpackRGB565(c, r, g, b);
    r = (r * factor) >> 8;
    g = (g * factor) >> 8;
    b = (b * factor) >> 8;
    return packRGB565(r, g, b);
}