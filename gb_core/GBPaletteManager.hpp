#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

class gb;

/**
 * GBColor: Wandelt Hex-RGB (0xRRGGBB) in Einzelkanäle
 * und das Game Boy Color Hardware-Format (RGB555) um.
 */
struct GBColor {
    uint8_t r, g, b;

    GBColor(uint32_t hex = 0x000000) {
        r = (hex >> 16) & 0xFF;
        g = (hex >> 8) & 0xFF;
        b = hex & 0xFF;
    }

    // Wandelt die Farbe in das 15-Bit Format (555) für den GB-Core um
    uint16_t toRGB555() const {
        return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10);
    }

    uint16_t toRGB565() const {
        return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
    }
};

/**
 * GBPalette: Hält die 4 Grundfarben einer Game Boy Palette.
 */
struct GBPalette {
    GBColor colors[4];
};

/**
 * GBPaletteManager: Verwaltet die Paletten-Auswahl.
 * Jede Instanz (1-16) besitzt ein eigenes Objekt dieser Klasse.
 */
class GBPaletteManager {
public:
    gb *ref_gb;

    GBPaletteManager(gb* ref) {
        ref_gb = ref;

        GlobalInit();
        currentPaletteKey = "Black and White";
    }


    void SetPalette(const std::string& key);

    void NextPalette();
    void PreviousPalette();

    const GBPalette& GetCurrent() const {
        return paletteDatabase.at(currentPaletteKey);
    }

    const std::string& GetCurrentKey() const {
        return currentPaletteKey;
    }

    // Statische Datenbank, die von allen Instanzen geteilt wird
    inline static std::map<std::string, GBPalette> paletteDatabase = {};

    // Initialisiert die Datenbank mit den Gambatte-Werten
    static void GlobalInit() {
        if (!paletteDatabase.empty()) return;

        // Hardware-Emulation
        paletteDatabase["Black and White"]  = { {0xFFFFFF, 0xAAAAAA, 0x555555, 0x000000} };
        paletteDatabase["Original DMG"]  = { {0xCADC06, 0x8BAC0F, 0x306230, 0x0F380F} };
        paletteDatabase["GB Pocket"]     = { {0xA7B19A, 0x86927C, 0x535F49, 0x2A3325} };
        paletteDatabase["GB Light"]      = { {0x01CBDF, 0x01B6D5, 0x269BAD, 0x00778D} };

        //Palettes from https://lospec.com/palette-list/tag/gameboy
        paletteDatabase["ICE CREAM GB"]      = { {0xfff6d3, 0xf9a875, 0xeb6b6f, 0x7c3f58} };
        paletteDatabase["AYY4"]      = { {0xf1f2da, 0xffce96, 0xff7777, 0x00303b} };
        paletteDatabase["2Bit DEMICHROME"]      = { {0xe9efec, 0xa0a08b, 0x555568, 0x211e20} };
        paletteDatabase["HOLLOW"]      = { {0xfafbf6, 0xc6b7be, 0x565a75, 0x0f0f1b} };
        paletteDatabase["Kirokaze Gameboy"]      = { {0xe2f3e4, 0x94e344, 0x46878f, 0x332c50} };
        paletteDatabase["Rustic GB"]      = { {0xedb4a1,0xa96868, 0x764462, 0x2c2137} };
        paletteDatabase["WISH GB"]      = { {0x8be5ff, 0x608fcf, 0x7550e8, 0x622e4c} };
        paletteDatabase["MIST GB"]      = { {0xc4f0c2, 0x5ab9a8, 0x1e606e, 0x2d1b00} };
        paletteDatabase["Crimson"]      = { {0x1b0326, 0x7a1c4b, 0xba5044, 0xeff9d6} };
        paletteDatabase["Velvet Cherry GB"]      = { 0x9775a6, 0x683a68, 0x412752, 0x2d162c };
        paletteDatabase["BLK AQU4"]      = { 0x9ff4e5, 0x00b9be, 0x005f8c, 0x002b59 };
        paletteDatabase["Nostalgia"]      = { 0xd0d058, 0xa0a840, 0x708028, 0x405010 };
        paletteDatabase["SpaceHaze"]      = { 0xf8e3c4, 0xcc3495, 0x6b1fb1, 0x0b0630 };
        paletteDatabase["Fiery Plague GB"]      = { 0x713141, 0x512839, 0x312137, 0x1a2129 };
        paletteDatabase["Gold GB"]      = { 0xcfab51, 0x9d654c, 0x4d222c, 0x210b1b };
        paletteDatabase["Coldfire GB"]      = { 0xf6c6a8, 0xd17c7c, 0x5b768d, 0x46425e };
        paletteDatabase["T-Lollipop"]      = { 0xe6f2ef, 0xf783b0, 0x3f6d9e, 0x151640 };
        paletteDatabase["Nymph GB"]      = { 0xa1ef8c, 0x3fac95, 0x446176, 0x2c2137 };
        paletteDatabase["Lospec GB"]      = { 0xc7c6c6, 0xc6d80, 0x382843, 0x000000};
        paletteDatabase["Bicycle"]      = { 0xf0f0f0, 0x8f9bf6, 0xab4646, 0x161616};
        paletteDatabase["Autumn Chill"]      = { 0xdad3af, 0xd58863, 0xc23a73, 0x2c1e74};
        paletteDatabase["HoneyGB"]      = { 0xe9f5da, 0xf0b695, 0x877286, 0x3e3a42};
        paletteDatabase["Slurry GB"]      = { 0xffffc7, 0xd4984a, 0x4e494c, 0x00303b};
        paletteDatabase["RGR-Papercut4"]      = { 0xcdb27b, 0x8b7d73, 0x3f3d47, 0x0c0c0d};
        paletteDatabase["Red Poster"]      = { 0xe8d6c0, 0x92938d, 0xa1281c, 0x000000};
        paletteDatabase["minty fresh"]      = { 0xfbffe0, 0x95c798, 0x856d52, 0x40332f};
        paletteDatabase["Andrade Gameboy"]      = { 0xe3eec0, 0xaeba89, 0x5e6745, 0x202020};
        paletteDatabase["Coral 4"]      = { 0xffd0a4, 0xf4949c, 0x7c9aac, 0x68518a};
        paletteDatabase["Modern 4"]      = { 0xf0e1d1, 0xd99741, 0x0c5066, 0x181b24};
        paletteDatabase["bluedream4"]      = { 0xecf2cb, 0x98d8b1, 0x4b849a, 0x1f285d};
        paletteDatabase["GB MŒBIUS"]      = { 0xf1e0cd, 0xffa49a, 0xda3467, 0x35333f};

        // Game Boy Color Palettes
        paletteDatabase["GBC P005"]      = { 0xFFFFFF, 0xFF9C00, 0xFF0000, 0x000000};
        paletteDatabase["GBC P007"]      = { 0xFFFFFF, 0xFFFF00, 0xFF0000, 0x000000};
        paletteDatabase["GBC P008"]      = { 0xA59CFF, 0xFFFF00, 0x006300, 0x000000};
        paletteDatabase["GBC P012"]      = { 0xFFFFFF, 0xFFAD63, 0x843100, 0x000000};
        paletteDatabase["GBC P016"]      = { 0xFFFFFF, 0xA5A5A5, 0x525252, 0x000000};
        paletteDatabase["GBC P017"]      = { 0xFFFFA5, 0xFF9494, 0x9494FF, 0x000000};
        paletteDatabase["GBC P01B"]      = { 0xFFFFFF, 0xFFCE00, 0x9C6300, 0x000000};
        paletteDatabase["GBC P005"]      = { 0xFFFFFF, 0xFF9C00, 0xFF0000, 0x000000};
        paletteDatabase["GBC P007"]      = { 0xFFFFFF, 0xFFFF00, 0xFF0000, 0x000000};
        paletteDatabase["GBC P008"]      = { 0xA59CFF, 0xFFFF00, 0x006300, 0x000000};
        paletteDatabase["GBC P012"]      = { 0xFFFFFF, 0xFFAD63, 0x843100, 0x000000};
        paletteDatabase["GBC P016"]      = { 0xFFFFFF, 0xA5A5A5, 0x525252, 0x000000};
        paletteDatabase["GBC P017"]      = { 0xFFFFA5, 0xFF9494, 0x9494FF, 0x000000};
        paletteDatabase["GBC P100"]      = { 0xFFFFFF, 0xADAD84, 0x42737B, 0x000000};
        paletteDatabase["GBC P110"]      = { 0xFFFFFF, 0xFF8484, 0x943A3A, 0x000000};
        paletteDatabase["GBC P11C"]      = { 0xFFFFFF, 0x7BFF31, 0x0063C5, 0x000000};
        paletteDatabase["GBC P20B"]      = { 0xFFFFFF, 0x63A5FF, 0x0000FF, 0x000000};
        paletteDatabase["GBC P20C"]      = { 0xFFFFFF, 0x8C8CDE, 0x52528C, 0x000000};

    }

private:
    std::string currentPaletteKey;
};

