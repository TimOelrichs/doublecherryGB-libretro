/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   IR TV REMOTE Emulation
   Copyright (C) 2023  Tim Oelrichs

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/* 

  Status: Works best with JVC IR protocol 
  Maybe it's not 100% accurate and needs research why the other protocols don't work

*/

#include "./include/tv_remote.hpp"
#include "../../libretro/libretro.h"
#include "./include/inline/inline_tv_remote_code.hpp"

extern void display_message_alt(std::string msg_str);
extern bool auto_random_tv_remote;


tv_remote::tv_remote(std::vector<gb*> gbs)
{
	v_gb.insert(v_gb.begin(), std::begin(gbs), std::end(gbs));
	reset();

	build_predefined_remotes();
}

void tv_remote::send_ir_signal(ir_signal* signal)
{
	v_gb[0]->receive_ir_signal(signal);
}

void tv_remote::process_ir()
{
	if (out_ir_signals.empty() && auto_random_tv_remote)
		gen_random_remote_signal();

	if (out_ir_signals.empty()) return;

	send_ir_signal(out_ir_signals[0]);
	out_ir_signals.erase(out_ir_signals.begin());

}

void tv_remote::gen_random_remote_signal()
{
	
	current_predefined_remote = rand() % 4; //JVC works best, so only generate JVC for now
	int command = rand() % remotes[current_predefined_remote]->commands.size();

	display_message_alt(get_remote_name() +
		"REMOTE: pressed " +
		remotes[current_predefined_remote]->commands[command]->name);

	byte code = (byte)remotes[current_predefined_remote]->commands[command]->code;
	build_signal_frame(code);

}

void tv_remote::reset()
{
	
	current_predefined_remote = 0;
}

void tv_remote::handle_special_hotkey(int key)
{

	out_ir_signals.clear();

	//Pressed SELECT
	if (key == 0x10) 
	{
		switch_tv_remote();
		return;
	}

	int command = key_2_remote_command(key);
	if (command == -1) return; 

	display_message_alt(get_remote_name() +
		"REMOTE: pressed " +
		remotes[current_predefined_remote]->commands[command]->name);

	byte code = (byte)remotes[current_predefined_remote]->commands[command]->code;
	build_signal_frame(code);

}

void tv_remote::build_predefined_remotes()
{

	remotes.push_back(new predefined_remote(JVC, 3, "JVC TV "));
	remotes.push_back(new predefined_remote(JVC, 34, "JVC CD Player "));
	remotes.push_back(new predefined_remote(JVC, 239, "JVC DVD Player "));
	remotes.push_back(new predefined_remote(JVC, 239, "JVC VCR "));
	remotes.push_back(new predefined_remote(NEC1, 64, "Toshiba TV "));
	remotes.push_back(new predefined_remote(NEC1, 69, "Toshiba DVD Player "));

	//JVC TV CODES
	remotes[0]->commands.push_back(new command_code("0", 32));
	remotes[0]->commands.push_back(new command_code("1", 33));
	remotes[0]->commands.push_back(new command_code("2", 34));
	remotes[0]->commands.push_back(new command_code("3", 35));
	remotes[0]->commands.push_back(new command_code("4", 36));
	remotes[0]->commands.push_back(new command_code("5", 37));
	remotes[0]->commands.push_back(new command_code("6", 38));
	remotes[0]->commands.push_back(new command_code("7", 39));
	remotes[0]->commands.push_back(new command_code("8", 40));
	remotes[0]->commands.push_back(new command_code("9", 41));
	remotes[0]->commands.push_back(new command_code("CHANNEL UP", 25));
	remotes[0]->commands.push_back(new command_code("CHANNEL DOWN", 24));
	remotes[0]->commands.push_back(new command_code("VOLUME UP", 30));
	remotes[0]->commands.push_back(new command_code("VOLUME DOWN", 31));
	remotes[0]->commands.push_back(new command_code("POWER", 23));
	remotes[0]->commands.push_back(new command_code("ENTER", 10));
	remotes[0]->commands.push_back(new command_code("MUTE", 28));

	//JVC CD PLAYER CODES
	remotes[1]->commands.push_back(new command_code("0", 32));
	remotes[1]->commands.push_back(new command_code("1", 33));
	remotes[1]->commands.push_back(new command_code("2", 34));
	remotes[1]->commands.push_back(new command_code("3", 35));
	remotes[1]->commands.push_back(new command_code("4", 36));
	remotes[1]->commands.push_back(new command_code("5", 37));
	remotes[1]->commands.push_back(new command_code("6", 38));
	remotes[1]->commands.push_back(new command_code("7", 39));
	remotes[1]->commands.push_back(new command_code("8", 40));
	remotes[1]->commands.push_back(new command_code("9", 41));
	remotes[1]->commands.push_back(new command_code("TRACK+", 25));
	remotes[1]->commands.push_back(new command_code("TRACK-", 24));
	remotes[1]->commands.push_back(new command_code("VOLUME UP", 30));
	remotes[1]->commands.push_back(new command_code("VOLUME DOWN", 31));
	remotes[1]->commands.push_back(new command_code("POWER", 0));
	remotes[1]->commands.push_back(new command_code("PLAY/PAUSE", 43));
	remotes[1]->commands.push_back(new command_code("STOP", 3));

	//JVC DVD PLAYER CODES
	remotes[2]->commands.push_back(new command_code("0", 96));
	remotes[2]->commands.push_back(new command_code("1", 97));
	remotes[2]->commands.push_back(new command_code("2", 98));
	remotes[2]->commands.push_back(new command_code("3", 99));
	remotes[2]->commands.push_back(new command_code("4", 100));
	remotes[2]->commands.push_back(new command_code("5", 101));
	remotes[2]->commands.push_back(new command_code("6", 102));
	remotes[2]->commands.push_back(new command_code("7", 103));
	remotes[2]->commands.push_back(new command_code("8", 104));
	remotes[2]->commands.push_back(new command_code("9", 105));
	remotes[2]->commands.push_back(new command_code("NEXT >>", 176));
	remotes[2]->commands.push_back(new command_code("PREV <<", 177));
	remotes[2]->commands.push_back(new command_code("VOLUME UP", 30));
	remotes[2]->commands.push_back(new command_code("VOLUME DOWN", 31));
	remotes[2]->commands.push_back(new command_code("POWER", 0));
	remotes[2]->commands.push_back(new command_code("PLAY", 76));
	remotes[2]->commands.push_back(new command_code("STOP", 67));

	//JVC VCR PLAYER CODES
	remotes[3]->commands.push_back(new command_code("0", 51));
	remotes[3]->commands.push_back(new command_code("1", 33));
	remotes[3]->commands.push_back(new command_code("2", 34));
	remotes[3]->commands.push_back(new command_code("3", 35));
	remotes[3]->commands.push_back(new command_code("4", 36));
	remotes[3]->commands.push_back(new command_code("5", 37));
	remotes[3]->commands.push_back(new command_code("6", 38));
	remotes[3]->commands.push_back(new command_code("7", 39));
	remotes[3]->commands.push_back(new command_code("8", 40));
	remotes[3]->commands.push_back(new command_code("9", 41));
	remotes[3]->commands.push_back(new command_code("FAST FORWARD >>", 6));
	remotes[3]->commands.push_back(new command_code("REWIND <<", 7));
	remotes[3]->commands.push_back(new command_code("PAUSE", 13));
	remotes[3]->commands.push_back(new command_code("RECORD", 204));
	remotes[3]->commands.push_back(new command_code("POWER", 11));
	remotes[3]->commands.push_back(new command_code("PLAY", 12));
	remotes[3]->commands.push_back(new command_code("STOP", 3));

	//Toshiba TV CODES
	remotes[4]->commands.push_back(new command_code("0", 0));
	remotes[4]->commands.push_back(new command_code("1", 1));
	remotes[4]->commands.push_back(new command_code("2", 2));
	remotes[4]->commands.push_back(new command_code("3", 3));
	remotes[4]->commands.push_back(new command_code("4", 4));
	remotes[4]->commands.push_back(new command_code("5", 5));
	remotes[4]->commands.push_back(new command_code("6", 6));
	remotes[4]->commands.push_back(new command_code("7", 7));
	remotes[4]->commands.push_back(new command_code("8", 8));
	remotes[4]->commands.push_back(new command_code("9", 9));
	remotes[4]->commands.push_back(new command_code("CHANNEL UP", 27));
	remotes[4]->commands.push_back(new command_code("CHANNEL DOWN", 31));
	remotes[4]->commands.push_back(new command_code("VOLUME UP", 26));
	remotes[4]->commands.push_back(new command_code("VOLUME DOWN", 30));
	remotes[4]->commands.push_back(new command_code("POWER", 126));
	remotes[4]->commands.push_back(new command_code("ENTER", 128));
	remotes[4]->commands.push_back(new command_code("MUTE", 16));

	//Toshiba DVD Player CODES
	remotes[5]->commands.push_back(new command_code("0", 10));
	remotes[5]->commands.push_back(new command_code("1", 1));
	remotes[5]->commands.push_back(new command_code("2", 2));
	remotes[5]->commands.push_back(new command_code("3", 3));
	remotes[5]->commands.push_back(new command_code("4", 4));
	remotes[5]->commands.push_back(new command_code("5", 5));
	remotes[5]->commands.push_back(new command_code("6", 6));
	remotes[5]->commands.push_back(new command_code("7", 7));
	remotes[5]->commands.push_back(new command_code("8", 8));
	remotes[5]->commands.push_back(new command_code("9", 9));
	remotes[5]->commands.push_back(new command_code("FWD >>", 19));
	remotes[5]->commands.push_back(new command_code("REV <<", 25));
	remotes[5]->commands.push_back(new command_code("SKIP >>", 36));
	remotes[5]->commands.push_back(new command_code("SKIP <<", 35));
	remotes[5]->commands.push_back(new command_code("POWER", 18));
	remotes[5]->commands.push_back(new command_code("PLAY", 21));
	remotes[5]->commands.push_back(new command_code("STOP", 20));


}

void tv_remote::build_signal_frame(byte code)
{
	out_ir_signals.clear();
	total_transmission_time = 0;
	v_gb[0]->get_cpu()->next_ir_clock = -2147483648;


		switch (current_remote_protocol)
		{
		case NEC1:
			build_nec_signal_frame(current_device_adress, code);
			break;
		case RC5X:
			build_rc5_signal_frame(current_device_adress, code);
			break;
		case RC6:
			build_rc6_signal_frame(current_device_adress, code);
			break;
		case JVC:
			build_jvc_signal_frame(current_device_adress, code);
			break;
		case SIRC:
			build_sirc_signal_frame(current_device_adress, code);
			break;
		case ITT:
			build_itt_signal_frame(current_device_adress, code);
			break;
		default:
			break;
		}

}

void tv_remote::build_nec_signal_frame(byte adress, byte code)
{

	//Header
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(9000)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(4500)));

	add_byte_to_out_ir_signals(adress);
	add_byte_to_out_ir_signals(~adress);
	add_byte_to_out_ir_signals(code);
	add_byte_to_out_ir_signals(~code);

	//End burst
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(562)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(40000)));

	/*
	//REPEAT CODE
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(9000)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(2250)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(562)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(108000 - 11812)));
	*/

}

void tv_remote::add_byte_to_out_ir_signals(byte data)
{
	byte out_bit = data;
	for (int i = 7; i >= 0; i--)
	{
		out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(562)));

		out_bit = ((data >> i) & 0x01);
		int duration = out_bit ? micro_seconds_to_clocks(2250-562) : micro_seconds_to_clocks(1125-562);
		out_ir_signals.push_back(new ir_signal(0, duration));

	}
}

void tv_remote::build_rc5_signal_frame(byte adress, byte command)
{
	//start pulses
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));

	//togglebit
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));

	total_transmission_time += (889 * 6);

	add_rc5_byte_to_out_ir_signals(adress, 5);
	add_rc5_byte_to_out_ir_signals(command, 7);

	//REPEAT
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(114000 - total_transmission_time)));

	//start pulses
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));

	//togglebit
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));

	total_transmission_time += (889 * 6);

	add_rc5_byte_to_out_ir_signals(adress, 5);
	add_rc5_byte_to_out_ir_signals(command, 6);


}

void tv_remote::add_rc5_byte_to_out_ir_signals(byte data, byte length)
{
	byte out_bit = data;

	for (int i = length-1; i >= 0; i--)
	{	
		out_bit = ((data >> i) & 0x01);
		if (out_bit) {
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
		}
		else
		{
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
		}

		total_transmission_time += (889 * 2);

	}
}



void tv_remote::build_rc6_signal_frame(byte adress, byte command)
{
	//HEADER
	//Leader pulse
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(2666)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
	//startbit, always 1
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(444)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(444)));
	//modebits (0,0,0) Mode0
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(444)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(444)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(444)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(444)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(444)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(444)));
	//trailerbit
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(889)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(889)));
	total_transmission_time += 2666 + (899 * 3) + (444 * 8);

	add_rc6_byte_to_out_ir_signals(adress);
	add_rc6_byte_to_out_ir_signals(command);
	
	//signal free tim
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(2666)));
}

void tv_remote::add_rc6_byte_to_out_ir_signals(byte data)
{
	byte out_bit = data;

	for (int i = 7; i >= 0; i--)
	{
		out_bit = ((data >> i) & 0x01);

		if (out_bit) {
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(444)));
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(444)));
		}
		else
		{
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(444)));
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(444)));
		}

		total_transmission_time += (444 * 2);

	}

}

void tv_remote::build_sirc_signal_frame(byte adress, byte command)
{
	//Header
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(2400)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(600)));
	add_sirc_byte_to_out_ir_signals(command, 7);
	add_sirc_byte_to_out_ir_signals(adress, 5);
}

void tv_remote::add_sirc_byte_to_out_ir_signals(byte data, byte length)
{
	byte out_bit = data;

	for (int i = length-1; i >= 0; i--)
	{
		out_bit = ((data >> i) & 0x01);
		if (out_bit) {
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(1200)));
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(600)));
		}
		else
		{
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(600)));
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(600)));
		}

	}
}

void tv_remote::build_jvc_signal_frame(byte adress, byte command)
{
	//Header
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(8400)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(4200)));

	total_transmission_time = 8400 + 4200;

	add_jvc_byte_to_out_ir_signals(adress);
	add_jvc_byte_to_out_ir_signals(command);

	//REPEAT
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(60000 - total_transmission_time)));
	total_transmission_time = 0;
	add_jvc_byte_to_out_ir_signals(adress);
	add_jvc_byte_to_out_ir_signals(command);

	//Header
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(8400)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(4200)));

	total_transmission_time = 8400 + 4200;

	add_jvc_byte_to_out_ir_signals(adress);
	add_jvc_byte_to_out_ir_signals(command);


}

void tv_remote::add_jvc_byte_to_out_ir_signals(byte data)
{
	byte out_bit = data;
	for (int i = 7; i >= 0; i--)
	{
		out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(562)));

		out_bit = ((data >> i) & 0x01);
		int duration = out_bit ? micro_seconds_to_clocks(2100-526) : micro_seconds_to_clocks(1050-526);
		out_ir_signals.push_back(new ir_signal(0, duration));
		
		total_transmission_time += 562;
		total_transmission_time += out_bit ? (2100 - 526) : (1050 - 526);

	}
}


void tv_remote::build_itt_signal_frame(byte adress, byte command)
{
	//Header
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(10)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(300-10)));

	add_itt_byte_to_out_ir_signals(adress, 4);
	add_itt_byte_to_out_ir_signals(command, 6);

	//End
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(10)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(300 - 10)));
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(10)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(100 - 10)));
}

void tv_remote::add_itt_byte_to_out_ir_signals(byte data, byte length)
{
	byte out_bit = data;

	for (int i = length - 1; i >= 0; i--)
	{
		out_bit = ((data >> i) & 0x01);
		if (out_bit) {
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(10)));
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(200-10)));
		}
		else
		{
			out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(10)));
			out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(100-10)));
		}

	}
}


std::string tv_remote::get_remote_name()
{
	return remotes[current_predefined_remote]->remote_name;
}

int tv_remote::key_2_remote_command(int key)
{
	//translate key to command
	switch (key)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		return key;
	case RETROK_KP_DIVIDE:
		return 10;
	case RETROK_KP_MULTIPLY:
		return 11;
	case RETROK_KP_PLUS:
		return 12;
	case RETROK_KP_MINUS:
		return 13;
	case RETROK_NUMLOCK:
		return 14;
	case RETROK_KP_ENTER:
		return 15;
	case RETROK_KP_PERIOD:
		return 16;
	default:
		break;
	}


	return -1;
}



void tv_remote::switch_tv_remote()
{

	++current_predefined_remote %= remotes.size();

	current_remote_protocol = remotes[current_predefined_remote]->protocol;
	current_device_adress = remotes[current_predefined_remote]->device_adress;

	display_message_alt("Switched to " + get_remote_name() + "Remote");

}
