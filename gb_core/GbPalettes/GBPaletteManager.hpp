#pragma once
#include <array>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>



enum class GBPalettePreset {
    DMG_Classic,
    Pocket_Neutral,
    Amber_Glow,
    Ice_Blue,
    Sepia_Tone,

    Gambatte_Classic,
    Gambatte_Green,
    Gambatte_LCD,

    SameBoy_BGB,
    SameBoy_AGS,
    SameBoy_GBB,

    ReShade_HighContrast,
    ReShade_WhiteBalance,
};

class GBPaletteManager {
public:
    GBPaletteManager();

    void setPalette(GBPalettePreset preset);
    void setPaletteByName(const std::string& name);

    uint16_t mapPixel(uint8_t grayLevel) const;

    std::vector<std::string> getAvailablePaletteNames() const;
    std::string getCurrentPaletteName() const;

    GBPalettePreset getCurrentPalette() const;

private:
    GBPalettePreset currentPalette;
    std::array<uint16_t, 4> palette;

    static const std::unordered_map<GBPalettePreset, std::array<uint16_t, 4>> predefinedPalettes;
    static const std::unordered_map<GBPalettePreset, std::string> presetNames;
    static const std::unordered_map<std::string, GBPalettePreset> nameToPreset;
};
