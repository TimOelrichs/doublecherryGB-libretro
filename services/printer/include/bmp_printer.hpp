// png_printer.hpp
#pragma once
#include "printer_interface.hpp"
#include <cstdint>

#pragma pack(push, 1)
struct BMPInfoHeader {
	uint32_t size = sizeof(BMPInfoHeader);
	int32_t width = 0;
	int32_t height = 0;
	uint16_t planes = 1;
	uint16_t bitCount = 24;  // 24-bit RGB
	uint32_t compression = 0;
	uint32_t imageSize = 0;
	int32_t xPixelsPerMeter = 2835;  // 72 DPI
	int32_t yPixelsPerMeter = 2835;
	uint32_t colorsUsed = 0;
	uint32_t importantColors = 0;
};

struct BMPHeader {
	uint16_t signature = 0x4D42;  // 'BM'
	uint32_t fileSize = 0;
	uint16_t reserved1 = 0;
	uint16_t reserved2 = 0;
	uint32_t dataOffset = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
};
#pragma pack(pop)

class BMPPrinter : public IPrinter {
public:
	std::string name() const override { return "BMPPrinter"; }
	bool print(const uint32_t* argb_pixels, int width, int height, ScaleTarget scale_target, Alignment alignment) override;
};



