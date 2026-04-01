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
   
   Status: WIP - not working yet
   Have to find the correct length for the ir pulses
   doc: https://shonumi.github.io/dandocs.html#zzh

*/



#include "./include/full_changer.hpp"


full_changer::full_changer(std::vector<gb*> gbs)
{
	v_gb.insert(v_gb.begin(), std::begin(gbs), std::end(gbs));
	reset();

	build_signal(0x01);
	
}

void full_changer::send_ir_signal(ir_signal* signal)
{
	v_gb[0]->receive_ir_signal(signal);
}

void full_changer::process_ir()
{

	if (out_ir_signals.empty()) return;

	send_ir_signal(out_ir_signals[0]);
	out_ir_signals.erase(out_ir_signals.begin());

}

void full_changer::reset()
{
}

void full_changer::handle_special_hotkey(int key)
{
}

void full_changer::build_signal(byte cosmic_character_id)
{
	//header
	out_ir_signals.push_back(new ir_signal(1,900));
	out_ir_signals.push_back(new ir_signal(0, 300));
	//data
	byte checksum = 0xFF - ~cosmic_character_id;
	add_byte_to_out_ir_signals(checksum);
	add_byte_to_out_ir_signals(~cosmic_character_id);
	//end
	out_ir_signals.push_back(new ir_signal(1, 900));
	out_ir_signals.push_back(new ir_signal(0, 300));

}

void full_changer::add_byte_to_out_ir_signals(byte data)
{
	byte out_bit = data;

	//LSB
	//for (int i = 0; i <= 7; i++)
	for (int i = 7; i >= 0; i--)
	{
		out_bit = ((data >> i) & 0x01);
		int duration_on = out_bit ? 600 : 300;
		int duration_off = out_bit ? 300 : 600;
		out_ir_signals.push_back(new ir_signal(1, duration_on));
		out_ir_signals.push_back(new ir_signal(0, duration_off));

	}

}
