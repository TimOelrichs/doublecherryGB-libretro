#include "GBPaletteManager.hpp"
#include "gb.h"

extern void display_message_alt(std::string msg_str);


void GBPaletteManager::NextPalette() {
        auto it = paletteDatabase.find(currentPaletteKey);
if (it != paletteDatabase.end()) {
    it++;
    if (it == paletteDatabase.end()) it = paletteDatabase.begin();
    currentPaletteKey = it->first;
}
ref_gb->get_lcd()->update_dmg_pallete();
ref_gb->get_renderer()->generateGradient();
display_message_alt(currentPaletteKey);
}

void GBPaletteManager::PreviousPalette() {
    auto it = paletteDatabase.find(currentPaletteKey);
    if (it != paletteDatabase.end()) {
        if (it == paletteDatabase.begin()) {
            it = paletteDatabase.end();
        }
        it--;
        currentPaletteKey = it->first;
    }
    ref_gb->get_lcd()->update_dmg_pallete();
    ref_gb->get_renderer()->generateGradient();
    display_message_alt(currentPaletteKey);
}

void GBPaletteManager::SetPalette(const std::string& key)
{
    if (paletteDatabase.count(key)) {
        currentPaletteKey = key;
    }
    ref_gb->get_lcd()->update_dmg_pallete();
    ref_gb->get_renderer()->generateGradient();
}
