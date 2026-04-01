#include "GBPaletteManager.hpp"


GBPaletteManager::GBPaletteManager() {
    setPalette(GBPalettePreset::DMG_Classic);
}

void GBPaletteManager::setPalette(GBPalettePreset preset) {
    currentPalette = preset;
    palette = predefinedPalettes.at(preset);
}

void GBPaletteManager::setPaletteByName(const std::string& name) {
    auto it = nameToPreset.find(name);
    if (it != nameToPreset.end())
        setPalette(it->second);
}

uint16_t GBPaletteManager::mapPixel(uint8_t grayLevel) const {
    // Gültigen Index sicherstellen (0-3 für Game Boy 4-Farben)
    const uint8_t index = grayLevel & 0x03;
    return palette[index];
}

std::string GBPaletteManager::getCurrentPaletteName() const {
    return presetNames.at(currentPalette);
}

GBPalettePreset GBPaletteManager::getCurrentPalette() const {
    return currentPalette;
}

std::vector<std::string> GBPaletteManager::getAvailablePaletteNames() const {
    std::vector<std::string> names;
    for (const auto& p : presetNames)
        names.push_back(p.second);
    return names;
}


// Definition der statischen Member
const std::unordered_map<GBPalettePreset, std::array<uint16_t, 4>> GBPaletteManager::predefinedPalettes = {
    {GBPalettePreset::DMG_Classic,       {0x7FFF, 0x5294, 0x294A, 0x0000}},
    {GBPalettePreset::Pocket_Neutral,    {0x6B5A, 0x5294, 0x39CE, 0x2108}},
    {GBPalettePreset::Amber_Glow,        {0x7FE0, 0x7D20, 0x6C00, 0x3000}},
    {GBPalettePreset::Ice_Blue,          {0x7FFF, 0x5EF7, 0x39CE, 0x0000}},
    {GBPalettePreset::Sepia_Tone,        {0x7BEF, 0x5A85, 0x3D08, 0x2108}},

    {GBPalettePreset::Gambatte_Classic,  {0x7FFF, 0x5294, 0x294A, 0x0000}},
    {GBPalettePreset::Gambatte_Green,    {0x03E0, 0x02A0, 0x0140, 0x0000}},
    {GBPalettePreset::Gambatte_LCD,      {0x7FFF, 0x5294, 0x294A, 0x0000}},

    {GBPalettePreset::SameBoy_BGB,       {0x7FFF, 0x5294, 0x294A, 0x0000}},
    {GBPalettePreset::SameBoy_AGS,       {0x7FFF, 0x5294, 0x294A, 0x0000}},
    {GBPalettePreset::SameBoy_GBB,       {0x7FFF, 0x5294, 0x294A, 0x0000}},

    {GBPalettePreset::ReShade_HighContrast, {0x7FFF, 0x5294, 0x294A, 0x0000}},
    {GBPalettePreset::ReShade_WhiteBalance, {0x7FFF, 0x5294, 0x294A, 0x0000}}
};

const std::unordered_map<GBPalettePreset, std::string> GBPaletteManager::presetNames = {
    {GBPalettePreset::DMG_Classic,       "DMG Classic"},
    {GBPalettePreset::Pocket_Neutral,    "Pocket Neutral"},
    {GBPalettePreset::Amber_Glow,        "Amber Glow"},
    {GBPalettePreset::Ice_Blue,          "Ice Blue"},
    {GBPalettePreset::Sepia_Tone,        "Sepia Tone"},

    {GBPalettePreset::Gambatte_Classic,  "Gambatte Classic"},
    {GBPalettePreset::Gambatte_Green,    "Gambatte Green"},
    {GBPalettePreset::Gambatte_LCD,      "Gambatte LCD"},

    {GBPalettePreset::SameBoy_BGB,       "SameBoy BGB"},
    {GBPalettePreset::SameBoy_AGS,       "SameBoy AGS"},
    {GBPalettePreset::SameBoy_GBB,       "SameBoy GBB"},

    {GBPalettePreset::ReShade_HighContrast, "ReShade High Contrast"},
    {GBPalettePreset::ReShade_WhiteBalance, "ReShade White Balance"}
};

const std::unordered_map<std::string, GBPalettePreset> GBPaletteManager::nameToPreset = {
    {"DMG Classic",         GBPalettePreset::DMG_Classic},
    {"Pocket Neutral",      GBPalettePreset::Pocket_Neutral},
    {"Amber Glow",          GBPalettePreset::Amber_Glow},
    {"Ice Blue",            GBPalettePreset::Ice_Blue},
    {"Sepia Tone",          GBPalettePreset::Sepia_Tone},

    {"Gambatte Classic",    GBPalettePreset::Gambatte_Classic},
    {"Gambatte Green",      GBPalettePreset::Gambatte_Green},
    {"Gambatte LCD",        GBPalettePreset::Gambatte_LCD},

    {"SameBoy BGB",         GBPalettePreset::SameBoy_BGB},
    {"SameBoy AGS",         GBPalettePreset::SameBoy_AGS},
    {"SameBoy GBB",         GBPalettePreset::SameBoy_GBB},

    {"ReShade High Contrast", GBPalettePreset::ReShade_HighContrast},
    {"ReShade White Balance", GBPalettePreset::ReShade_WhiteBalance}
};