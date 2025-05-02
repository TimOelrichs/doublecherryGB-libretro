// png_printer.hpp
#pragma once
#include "printer_interface.hpp"

class PNGPrinter : public IPrinter {
public:
    std::string name() const override { return "PNGPrinter"; }

    bool print(const uint32_t* argb_pixels, int width, int height, ScaleTarget scale_target, Alignment alignment) override;
};
