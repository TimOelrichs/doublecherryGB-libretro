#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

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
    GBPaletteManager() {
        // Sicherstellen, dass die globale Datenbank existiert
        GlobalInit();
        // Standard-Palette für diese Instanz
        currentPaletteKey = "Original DMG";
    }

    // Setzt die Palette direkt über den Namen (Key)
    void SetPalette(const std::string& key) {
        if (paletteDatabase.count(key)) {
            currentPaletteKey = key;
        }
    }

    // Wechselt zur nächsten Palette in der Liste (für RB Button)
    void NextPalette() {
        auto it = paletteDatabase.find(currentPaletteKey);
        if (it != paletteDatabase.end()) {
            it++;
            if (it == paletteDatabase.end()) it = paletteDatabase.begin();
            currentPaletteKey = it->first;
        }
    }

    // Wechselt zur vorherigen Palette (für LB Button)
    void PreviousPalette() {
        auto it = paletteDatabase.find(currentPaletteKey);
        if (it != paletteDatabase.end()) {
            if (it == paletteDatabase.begin()) {
                it = paletteDatabase.end();
            }
            it--;
            currentPaletteKey = it->first;
        }
    }

    // Gibt die aktuell gewählte Palette zurück
    const GBPalette& GetCurrent() const {
        return paletteDatabase.at(currentPaletteKey);
    }

    // Gibt den Namen der aktuellen Palette zurück
    const std::string& GetCurrentKey() const {
        return currentPaletteKey;
    }

    // Statische Datenbank, die von allen Instanzen geteilt wird
    inline static std::map<std::string, GBPalette> paletteDatabase = {};

    // Initialisiert die Datenbank mit den Gambatte-Werten
    static void GlobalInit() {
        if (!paletteDatabase.empty()) return;

        // Hardware-Emulation
        paletteDatabase["Original DMG"]  = { {0x578200, 0x317400, 0x005121, 0x00420C} };
        paletteDatabase["Original DMG Green"]  = { {0x578200, 0x317400, 0x005121, 0x00420C} };
        paletteDatabase["GB Pocket"]     = { {0xA7B19A, 0x86927C, 0x535F49, 0x2A3325} };
        paletteDatabase["GB Light"]      = { {0x01CBDF, 0x01B6D5, 0x269BAD, 0x00778D} };

        // GBC Interne Paletten (für DMG Spiele auf GBC Hardware)
        paletteDatabase["GBC Green"]     = { {0xFFFFFF, 0x52FF00, 0xFF4200, 0x000000} };
        paletteDatabase["GBC Orange"]    = { {0xFFFFFF, 0xFF9C00, 0xFF0000, 0x000000} };
        paletteDatabase["GBC Red"]       = { {0xFFFFFF, 0xFFFF00, 0xFF0000, 0x000000} };
        paletteDatabase["GBC Dark Red"]  = { {0xA59CFF, 0xFFFF00, 0x006300, 0x000000} };
        paletteDatabase["GBC Brown"]     = { {0xFFFFFF, 0xFFAD63, 0x843100, 0x000000} };
        paletteDatabase["GBC Inverted"]  = { {0x000000, 0x008484, 0xFFDE00, 0xFFFFFF} };
        paletteDatabase["GBC Greyscale"] = { {0xFFFFFF, 0xA5A5A5, 0x525252, 0x000000} };
        paletteDatabase["GBC Pastel"]    = { {0xFFFFA5, 0xFF9494, 0x9494FF, 0x000000} };
        paletteDatabase["GBC Gold"]      = { {0xFFFFFF, 0xFFCE00, 0x9C6300, 0x000000} };
        paletteDatabase["GBC Aqua"]      = { {0xFFFFFF, 0xADAD84, 0x42737B, 0x000000} };
        paletteDatabase["GBC Blue"]      = { {0xFFFFFF, 0x63A5FF, 0x0000FF, 0x000000} };
        paletteDatabase["GBC Dark Blue"] = { {0xFFFFFF, 0x8C8CDE, 0x52528C, 0x000000} };
        paletteDatabase["GBC Pink"]      = { {0xFFFFFF, 0xFF8484, 0x943A3A, 0x000000} };
        paletteDatabase["GBC Forest"]    = { {0xFFFFFF, 0x7BFF31, 0x0063C5, 0x000000} };
        paletteDatabase["GBC Sunset"]    = { {0xB5B5FF, 0xFFFF94, 0xAD5A42, 0x000000} };
    }

private:
    std::string currentPaletteKey;
};