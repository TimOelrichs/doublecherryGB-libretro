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

void GBPaletteManager::GlobalInit() {
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
    paletteDatabase["Crimson"]      = { {0xeff9d6, 0xba5044, 0x7a1c4b, 0x1b0326} };
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

    paletteDatabase["Cozy Camp"]      = { 0xf8eacf, 0xc07e53, 0x30455c, 0x071721};
    paletteDatabase["Winter Forest"]      = { 0xeeffff, 0x55bbdd, 0x007744, 0x332200};
    paletteDatabase["Lunar"]      = { 0xe5dac2, 0x9a7ca7, 0x484267, 0x0f1327};
    paletteDatabase["Rustic GB"]      = { 0xedb4a1, 0xa96868, 0x764462, 0x2c2137};
    paletteDatabase["2 Bit Matrix"]      = { 0xf2fff2, 0xadd9bc, 0x5b8c7c, 0x0d1a1a};
    paletteDatabase["Dusty4"]      = { 0xf5f6df, 0x5a8f78, 0x3a5068, 0x372a51};
    paletteDatabase["Slurry GB"]      = { 0xffffc7, 0xd4984a, 0x4e494c, 0x00303};
    paletteDatabase["Cosmic Cream GB"]      = { 0xfffab2, 0xffba5a, 0x615fed, 0x220780};
    paletteDatabase["Metallic GB"]      = { 0xc5dbd4, 0x778e98, 0x41485d, 0x221e31};
    paletteDatabase["THE INTERNET RAISES YOU PALETTE"]      = { 0xe4d6cf, 0x9b9880, 0x7b505c, 0x322623};
    paletteDatabase["Purpledawn"]      = { 0xeefded, 0x9a7bbc, 0x2d757e, 0x001b2e};
    paletteDatabase["Amber CRTGB"]      = { 0xfed018, 0xd35600, 0x5e1210, 0x0d0405};
    paletteDatabase["Cave4"]      = { 0xe4cbbf, 0x938282, 0x4f4e80, 0x2c0016};
    paletteDatabase["STAR POP"]      = { 0xffebe5, 0xffa3d6, 0x64b9ca, 0x674577};
    paletteDatabase["MOKKY"]      = { 0xcca66e, 0x99683d, 0x664930, 0x332920};
    paletteDatabase["EARTH GB"]      = { 0xf5f29e, 0xacb965, 0xb87652, 0x774346};
    paletteDatabase["Caramel Autumn"]      = { 0xfff4b8, 0xff8b40, 0xa22fc9, 0x290143};
    paletteDatabase["Dream Candy"]      = { 0xffffff, 0xf0d063, 0xd075b7, 0x442d6e};
    paletteDatabase["Pumpkin GB"]      = { 0xf7db7e, 0xe06e16, 0x19692c, 0x142b23};
    paletteDatabase["RED IS DEAD"]      = { 0xfffcfe, 0xff0015, 0x860020, 0x11070a};
    paletteDatabase["VIVID 2BIT SCREAM"]      = { 0xcaf532, 0x74af34, 0x5c4fa3, 0x561d17};
    paletteDatabase["SNOWFLAKE4"]      = { 0xe7edeb, 0x8ecece, 0x62a1c7, 0x3f6ecc};
    paletteDatabase["CANDYPOP!"]      = { 0xeebff5, 0x9e81d0, 0x854576, 0x301221};
    paletteDatabase["LEMON-LIME"]      = { 0xfff37b, 0x5fcc86, 0x39809c, 0x28375b};
    paletteDatabase["MONDAY4"]      = { 0xac8876, 0x906776, 0x634d69, 0x353546};
    paletteDatabase["RABBIT5PM"]      = { 0xffe7cd, 0xe4a39f, 0x629098, 0x34c3457};
    paletteDatabase["HEXPRESS4"]      = { 0xedf8c8, 0xbebc6a, 0x9b6859, 0x553840};
    paletteDatabase["ST GAMEBOY DELUXE"]      = { 0xf4f5e9, 0xa8bf4c, 0x442a8c, 0x09091a};
    paletteDatabase["MUDDYSAND"]      = { 0xe6d69c, 0xb4a56a, 0x7b7162, 0x393829};
    paletteDatabase["PUMPKIN PIE"]      = { 0xfff2e5, 0xf7b58c, 0x739ca1, 0x5d446c};
    paletteDatabase["MONOLOGOS"]      = { 0xababab, 0x7a7272, 0x372c38, 0x211f1f};
    paletteDatabase["THE ROAR OF THE OWL"]      = { 0x83e6cd, 0x66adc6, 0x4f53ab, 0x3e2469};
    paletteDatabase["COFFEE CAFE"]      = { 0xcc9e7a, 0x99745c, 0x734d45, 0x4d302e};
    paletteDatabase["GOOSEBERRY THISTLE"]      = { 0xbfdce1, 0xb883b9, 0x6c56ab, 0x48376c};
    paletteDatabase["BLUEBERRY GB"]      = { 0xb0ecf9, 0xaa73e0, 0x6c2e53, 0x280b0b};
    paletteDatabase["MUSHBLOOM"]      = { 0xfff9f1, 0xcaaea3, 0x8b6f7b, 0x2b4158};
    paletteDatabase["GB MORRIS"]      = { 0xe5d8ac, 0x7db3ab, 0x7c714a, 0x264b38};
    paletteDatabase["ACIENT SCROLL"]      = { 0xe5dab9, 0xbebba1, 0x738e85, 0x2e4252};
    paletteDatabase["ST GAMEBOY MAGMA"]      = { 0xf2c549, 0xbf531d, 0x661f1a, 0x1a0f16};
    paletteDatabase["QAMEBOY"]      = { 0xb2c27d, 0x739a56, 0x42665a, 0x353d46};
    paletteDatabase["SOULSCAPE"]      = { 0x42f4af, 0x1cb099, 0x0b586e, 0x051e45};
    paletteDatabase["SEXY04"]      = { 0xdac4b9, 0xe779a0, 0x9e2e9d, 0x141852};
    paletteDatabase["PEACH BLIZZARD"]      = { 0xfbd6ad, 0xf19386, 0xa54371, 0x4c388a};


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

void GBPaletteManager::SetPalette(const std::string& key)
{
    if (paletteDatabase.count(key)) {
        currentPaletteKey = key;
    }
    ref_gb->get_lcd()->update_dmg_pallete();
    ref_gb->get_renderer()->generateGradient();
}
