
// DoubleCherryGB Gameboy Printer ported from GBE+ 
// Ported by Tim Oelrichs

// GB Enhanced Copyright Daniel Baxter 2016
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : sio.cpp
// Date : April 30, 2016
// Description : Game Boy Serial Input-Output emulation
//
// Sets up SDL networking
// Emulates Gameboy-to-Gameboy data transfers
// Emulates various SIO devices like GB Printer and more


#include "./include/gameboy_printer.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


extern ScaleTarget gb_printer_png_scale_mode;
extern Alignment gb_printer_png_alignment;


gameboy_printer::gameboy_printer()
{
	reset();
}

byte gameboy_printer::receive_from_linkcable(byte data)
{
	//Check for magic bytes at any time during initial transfer
	//Pokemon Pinball sometimes sends 0x10 0x33. Needs hardware verification on how this works. Treat it as valid for now.
	if (((last_transfer == 0x88) || (last_transfer == 0x10)) && (data == 0x33) && (packet_size <= 6))
	{
		current_state = GBP_AWAITING_PACKET;
		packet_buffer.clear();
		packet_buffer.push_back(0x88);
		packet_size = 1;
	}

	switch (current_state)
	{
		//Receive packet data
	case GBP_AWAITING_PACKET:
	{
		//Push data to packet buffer	
		packet_buffer.push_back(data);
		packet_size++;

		//Check for magic number 0x88 0x33
		if ((packet_size == 2) && (packet_buffer[0] == 0x88) && (packet_buffer[1] == 0x33))
		{
			//Move to the next state
			current_state = GBP_RECEIVE_COMMAND;
		}

		//If magic number not found, reset
		else if (packet_size == 2)
		{
			packet_size = 1;
			byte temp = packet_buffer[1];
			packet_buffer.clear();
			packet_buffer.push_back(temp);
		}

		//Send data back to GB + IRQ
		return 0x0;
		break;
	}
	//Receive command
	case GBP_RECEIVE_COMMAND:

		//Push data to packet buffer
		packet_buffer.push_back(data);
		packet_size++;

		//Grab command. Check to see if the value is a valid command
		command = packet_buffer.back();

		//Abort if invalid command, wait for a new packet
		if ((command != 0x1) && (command != 0x2) && (command != 0x4) && (command != 0xF)
			&& (command != 0x88) && (command != 0x10))
		{
			//std::cout << "SIO::Warning - Invalid command sent to GB Printer -> 0x" << std::hex << (unsigned int)command << "\n";
			current_state = GBP_AWAITING_PACKET;
		}

		else
		{
			//Move to the next state
			current_state = GBP_RECEIVE_COMPRESSION_FLAG;
		}

		//Send data back to GB + IRQ
		return 0x0;
		break;

		//Receive compression flag
	case GBP_RECEIVE_COMPRESSION_FLAG:

		//Push data to packet buffer
		packet_buffer.push_back(data);
		packet_size++;

		//Grab compression flag
		compression_flag = packet_buffer.back();

		//Move to the next state
		current_state = GBP_RECEIVE_LENGTH;

		//Send data back to GB + IRQ
		return 0x0;
		break;

		//Receive data length
	case GBP_RECEIVE_LENGTH:

		//Push data to packet buffer
		packet_buffer.push_back(data);
		packet_size++;

		//Grab LSB of data length
		if (packet_size == 5)
		{
			data_length = 0;
			data_length |= packet_buffer.back();
		}

		//Grab MSB of the data length, move to the next state
		else if (packet_size == 6)
		{
			packet_size = 0;
			data_length |= (packet_buffer.back() << 8);

			//Receive data only if the length is non-zero
			if (data_length > 0) { current_state = GBP_RECEIVE_DATA; }

			//Otherwise, move on to grab the checksum
			else { current_state = GBP_RECEIVE_CHECKSUM; }
		}

		//Send data back to GB + IRQ
		return 0x0;
		break;

		//Receive data
	case GBP_RECEIVE_DATA:

		//Push data to packet buffer
		packet_buffer.push_back(data);
		packet_size++;

		//Once the specified amount of data is transferred, move to the next stage
		if (packet_size == data_length)
		{
			packet_size = 0;
			current_state = GBP_RECEIVE_CHECKSUM;
		}

		//Send data back to GB + IRQ
		return 0x0;
		break;

		//Receive checksum
	case GBP_RECEIVE_CHECKSUM:

		//Push data to packet buffer
		packet_buffer.push_back(data);
		packet_size++;

		//Grab LSB of checksum
		if (packet_size == 1)
		{
			checksum = 0;
			checksum |= packet_buffer.back();
		}

		//Grab MSB of the checksum, move to the next state
		else if (packet_size == 2)
		{
			packet_size = 0;
			checksum |= (packet_buffer.back() << 8);
			current_state = GBP_ACKNOWLEDGE_PACKET;

			word checksum_match = 0;

			//Calculate checksum
			for (unsigned int x = 2; x < (packet_buffer.size() - 2); x++)
			{
				checksum_match += packet_buffer[x];
			}

			if (checksum_match != checksum) { status |= 0x1; }
			else { status &= ~0x1; }
		}

		//Send data back to GB + IRQ
		return 0x0;
		break;

		//Acknowledge packet and process command
	case GBP_ACKNOWLEDGE_PACKET:

		//GB Printer expects 2 0x0s, only continue if given them
		if (data == 0)
		{
			//Push data to packet buffer
			packet_buffer.push_back(data);
			packet_size++;

			//Send back 0x81 to GB + IRQ on 1st 0x0
			if (packet_size == 1)
			{
				return 0x81;

			}

			//Send back current status to GB + IRQ on 2nd 0x0, begin processing command
			else if (packet_size == 2)
			{
				execute_command();

				packet_buffer.clear();
				packet_size = 0;

				return status;
			}
		}
		break;
	}
	last_transfer = data;

}

void gameboy_printer::execute_command()
{
	switch (command)
	{
		//Initialize command
	case 0x1:
		status = 0x0;
		strip_count = 0;

		//Clear internal scanline data
		scanline_buffer.clear();
		scanline_buffer.resize(0x5A00, 0x0);

		break;

		//Print command
	case 0x2:
		print_image();
		status = 0x4;

		break;

		//Data process command
	case 0x4:
		data_process();

		//Only set Ready-To-Print status if some actual data was received
		if (strip_count != 0) { status = 0x8; }

		break;

		//Status command
	case 0xF:
		status |= 0;

		break;

	default:
		//std::cout << "SIO::Warning - Invalid command sent to GB Printer -> 0x" << std::hex << (unsigned int)command << "\n";
		break;
	}

	current_state = GBP_AWAITING_PACKET;
}


void gameboy_printer::data_process()
{
	unsigned int data_pointer = 6;
	unsigned int pixel_counter = strip_count * 2560;
	byte tile_pixel = 0;

	if (strip_count >= 9)
	{
		for (unsigned int x = 0; x < 2560; x++) { scanline_buffer.push_back(0x0); }
	}

	//Process uncompressed dot data
	if (!compression_flag)
	{
		//Cycle through all tiles given in the data, 40 in all
		for (unsigned int x = 0; x < 40; x++)
		{
			//Grab 16-bytes representing each tile, 2 bytes at a time
			for (unsigned int y = 0; y < 8; y++)
			{
				//Move pixel counter down one row in the tile
				pixel_counter = (strip_count * 2560) + ((x % 20) * 8) + (160 * y);
				if (x >= 20) { pixel_counter += 1280; }

				//Grab 2-bytes representing 8x1 section
				word tile_data = (packet_buffer[data_pointer + 1] << 8) | packet_buffer[data_pointer];
				data_pointer += 2;

				//Determine color of each pixel in that 8x1 section
				for (int z = 7; z >= 0; z--)
				{
					//Calculate raw value of the tile's pixel
					tile_pixel = ((tile_data >> 8) & (1 << z)) ? 2 : 0;
					tile_pixel |= (tile_data & (1 << z)) ? 1 : 0;

					switch (tile_pixel)
					{
					case 0:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[0];
						break;

					case 1:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[1];
						break;

					case 2:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[2];
						break;

					case 3:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[3];
						break;
					}
				}
			}
		}
	}

	//Process compressed dot data
	else
	{
		std::vector<byte> dot_data;
		byte data = 0;

		//Cycle through all the compressed data and calculate the RLE
		while (data_pointer < (data_length + 6))
		{
			//Grab MSB of 1st byte in the run, if 1 the run is compressed, otherwise it is an uncompressed run
			byte data = packet_buffer[data_pointer++];

			//Compressed run
			if (data & 0x80)
			{
				byte length = (data & 0x7F) + 2;
				data = packet_buffer[data_pointer++];

				for (unsigned int x = 0; x < length; x++) { dot_data.push_back(data); }
			}

			//Uncompressed run
			else
			{
				byte length = (data & 0x7F) + 1;

				for (unsigned int x = 0; x < length; x++)
				{
					data = packet_buffer[data_pointer++];
					dot_data.push_back(data);
				}
			}
		}

		data_pointer = 0;

		//Cycle through all tiles given in the data, 40 in all
		for (unsigned int x = 0; x < 40; x++)
		{
			//Grab 16-bytes representing each tile, 2 bytes at a time
			for (unsigned int y = 0; y < 8; y++)
			{
				//Move pixel counter down one row in the tile
				pixel_counter = (strip_count * 2560) + ((x % 20) * 8) + (160 * y);
				if (x >= 20) { pixel_counter += 1280; }

				//Grab 2-bytes representing 8x1 section
				word tile_data = (dot_data[data_pointer + 1] << 8) | dot_data[data_pointer];
				data_pointer += 2;

				//Determine color of each pixel in that 8x1 section
				for (int z = 7; z >= 0; z--)
				{
					//Calculate raw value of the tile's pixel
					tile_pixel = ((tile_data >> 8) & (1 << z)) ? 2 : 0;
					tile_pixel |= (tile_data & (1 << z)) ? 1 : 0;

					switch (tile_pixel)
					{
					case 0:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[0];
						break;

					case 1:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[1];
						break;

					case 2:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[2];
						break;

					case 3:
						scanline_buffer[pixel_counter++] = DMG_BG_PAL[3];
						break;
					}
				}
			}
		}
	}

	//Only increment strip count if we actually received data
	if (data_length != 0) { strip_count++; }
}

void gameboy_printer::print_image()
{
	unsigned int height = (16 * strip_count);
	unsigned int img_size = 160 * height;

	byte margin_top = (packet_buffer[7] & 0xF0);
	byte margin_bottom = (packet_buffer[7] & 0x0F);
	bool print_full_pix = (!margin_top && margin_bottom) ? true : false;

	//Clear full printer buffer if new strip detected or it gets way too large (50 160x144 strips)
	if ((margin_top) || (full_buffer.size() >= 0x119400)) { full_buffer.clear(); }

	//Set up printing palette
	byte data_pal = packet_buffer[8];

	pal[0] = data_pal & 0x3;
	pal[1] = (data_pal >> 2) & 0x3;
	pal[2] = (data_pal >> 4) & 0x3;
	pal[3] = (data_pal >> 6) & 0x3;

	//Calculate printing exposure (contrast for final pixels) and set color accordingly
	short exposure = (packet_buffer[9] & 0x7F);
	unsigned int print_color_0 = (DMG_BG_PAL[0] & 0xFF);
	unsigned int print_color_1 = (DMG_BG_PAL[1] & 0xFF);
	unsigned int print_color_2 = (DMG_BG_PAL[2] & 0xFF);
	unsigned int print_color_3 = (DMG_BG_PAL[3] & 0xFF);

	double diff = (0x40 - exposure) * (25.0 / 64);
	diff = ((100 + diff) / 100);


	print_color_0 = std::min(std::max(int(print_color_0 * diff), 0), 255);
	print_color_1 = std::min(std::max(int(print_color_1 * diff), 0), 255);
	print_color_2 = std::min(std::max(int(print_color_2 * diff), 0), 255);
	print_color_3 = std::min(std::max(int(print_color_3 * diff), 0), 255);

	/*
	if ((print_color_0 * diff) > 255) { print_color_0 = 255; }
	else if ((print_color_0 * diff) < 0) { print_color_0 = 0; }
	else { print_color_0 *= diff; }
	print_color_0 = 0xFF000000 | (print_color_0 << 16) | (print_color_0 << 8) | print_color_0;

	if ((print_color_1 * diff) > 255) { print_color_1 = 255; }
	else if ((print_color_1 * diff) < 0) { print_color_1 = 0; }
	else { print_color_1 *= diff; }
	print_color_1 = 0xFF000000 | (print_color_1 << 16) | (print_color_1 << 8) | print_color_1;

	if ((print_color_2 * diff) > 255) { print_color_2 = 255; }
	else if ((print_color_2 * diff) < 0) { print_color_2 = 0; }
	else { print_color_2 *= diff; }
	print_color_2 = 0xFF000000 | (print_color_2 << 16) | (print_color_2 << 8) | print_color_2;

	if ((print_color_3 * diff) > 255) { print_color_3 = 255; }
	else if ((print_color_3 * diff) < 0) { print_color_3 = 0; }
	else { print_color_3 *= diff; }
	print_color_3 = 0xFF000000 | (print_color_3 << 16) | (print_color_3 << 8) | print_color_3;
	*/

	for (unsigned int x = 0; x < img_size; x++)
	{
		//Convert pixels to printer palette if necessary
		byte tile_pixel = 0;

		if (scanline_buffer[x] == DMG_BG_PAL[0]) { tile_pixel = 0; }
		else if (scanline_buffer[x] == DMG_BG_PAL[1]) { tile_pixel = 1; }
		else if (scanline_buffer[x] == DMG_BG_PAL[2]) { tile_pixel = 2; }
		else if (scanline_buffer[x] == DMG_BG_PAL[3]) { tile_pixel = 3; }

		switch (pal[tile_pixel])
		{
		case 0:
			scanline_buffer[x] = print_color_0;
			break;

		case 1:
			scanline_buffer[x] = print_color_1;
			break;

		case 2:
			scanline_buffer[x] = print_color_2;
			break;

		case 3:
			scanline_buffer[x] = print_color_3;
			break;
		}

		//Fill full print buffer continuously
		full_buffer.push_back(scanline_buffer[x]);
	}

	

	strip_count = 0;

	/*

	//Print full combined strip if detected
	if (print_full_pix)
	{
		height = full_buffer.size() / 160;
		SDL_Surface* full_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 160, height, 32, 0, 0, 0, 0);

		if (SDL_MUSTLOCK(full_screen)) { SDL_LockSurface(full_screen); }
		unsigned int* full_pixel_data = (unsigned int*)full_screen->pixels;

		for (unsigned int x = 0; x < full_buffer.size(); x++) { full_pixel_data[x] = full_buffer[x]; }
		full_buffer.clear();

		//Unlock source surface
		if (SDL_MUSTLOCK(full_screen)) { SDL_UnlockSurface(full_screen); }

		filename = "full_" + filename;

		SDL_SaveBMP(full_screen, filename.c_str());
		SDL_FreeSurface(full_screen);
	}

	*/


	unsigned int width = 160;
	unsigned int gb_height = full_buffer.size() / width;


	if (
		PrinterRegistry::current()->print(reinterpret_cast<const uint32_t*>(full_buffer.data()), width, height, gb_printer_png_scale_mode, gb_printer_png_alignment)
		)
		display_message("Printed successfull to ./Screenshots");
	else
		display_message("Something went wrong.");


}

void gameboy_printer::reset()
{
	packet_size = 0;
	current_state = GBP_AWAITING_PACKET;

	command = compression_flag = strip_count = data_length = checksum = status = last_transfer = 0;
}



