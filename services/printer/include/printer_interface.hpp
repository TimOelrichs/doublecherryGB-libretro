// printer_interface.hpp
#pragma once
#include "./image_scaler.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>


extern bool gb_printer_png_scale_enabled;

struct RGB { uint8_t r, g, b; };

class IPrinter {
public:
    virtual ~IPrinter() = default;

    virtual std::string name() const = 0;
    virtual bool print(const uint32_t* argb_pixels, int width, int height, ScaleTarget scale_target, Alignment alignment) = 0;
};


static std::string get_timestamped_filename() {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);

	std::ostringstream oss;
	oss << "./screenshots/gb_print_";  // relative, plattformunabhängig
	oss << std::put_time(localTime, "%Y%m%d_%H%M%S");
	//oss << ".png";

	return oss.str();
}