#pragma once
#include "../../gb.h"
#include "services/printer/include/printer_registry.hpp"


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

extern void display_message(std::string msg_str);


class gameboy_printer : public I_linkcable_target
	
{

public:
	gameboy_printer();

	byte receive_from_linkcable(byte data) override;


private:

	void execute_command();
	void data_process();
	void print_image();
	void reset();




	std::vector <unsigned int> scanline_buffer, full_buffer, out_pixel_data;
	std::vector<RGB> rgb_buffer; 
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






