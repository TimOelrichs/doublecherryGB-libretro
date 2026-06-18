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
    static void GlobalInit();

private:
    std::string currentPaletteKey;
};

