#include "../../gb.h"
#include <cstdint>

enum printer_state
{
	GBP_AWAITING_PACKET,
	GBP_RECEIVE_COMMAND,
	GBP_RECEIVE_COMPRESSION_FLAG,
	GBP_RECEIVE_LENGTH,
	GBP_RECEIVE_DATA,
	GBP_RECEIVE_CHECKSUM,
	GBP_ACKNOWLEDGE_PACKET,
};




class gameboy_printer : public I_linkcable_target
	
{

public:
	gameboy_printer();

	byte receive_from_linkcable(byte data) override;


private:

	void execute_command();
	void data_process();
	void print_image_to_BMP();
	void reset();

	std::vector <unsigned int> scanline_buffer, full_buffer, out_pixel_data;
	std::vector <byte> packet_buffer;
	unsigned int packet_size;
	printer_state current_state;

	byte command;
	byte compression_flag;
	byte strip_count;
	word data_length;
	word checksum;
	byte status;
	byte pal[4];

	byte last_transfer; 

	//Default Gameboy BG palettes
	unsigned int DMG_BG_PAL[4] = { 0xFFFFFFFF, 0xFFC0C0C0, 0xFF606060, 0xFF000000 };
};

#pragma pack(push, 1)
struct BMPHeader {
	uint16_t fileType{ 0x4D42 };  // 'BM'
	uint32_t fileSize{ 0 };       // Size of the file (in bytes)
	uint16_t reserved1{ 0 };      // Reserved
	uint16_t reserved2{ 0 };      // Reserved
	uint32_t offsetData{54};    // Start position of pixel data
};

struct BMPInfoHeader {
	uint32_t size{ 40 };          // Size of this header (40 bytes)
	int32_t width{ 0 };           // Width of the bitmap in pixels
	int32_t height{ 0 };          // Height of the bitmap in pixels
	uint16_t planes{ 1 };         // Number of color planes (must be 1)
	uint16_t bitCount{ 24 };      // Number of bits per pixel
	uint32_t compression{ 0 };    // Compression type (0 = uncompressed)
	uint32_t sizeImage{ 0 };      // Size of pixel data (can be 0 for uncompressed)
	int32_t xPixelsPerMeter{ 0 }; // Horizontal resolution
	int32_t yPixelsPerMeter{ 0 }; // Vertical resolution
	uint32_t colorsUsed{ 0 };     // Number of colors used (0 = all)
	uint32_t importantColors{ 0 }; // Number of important colors (0 = all)
};
#pragma pack(pop)



