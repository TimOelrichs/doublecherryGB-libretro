/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Burger Time Deluxe Multiplayer Hack
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
* Status: WIP - not finished yet
*/


#include "gb.h"
#include <array> 
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "./include/hack_4p_burger_time_deluxe.hpp"


hack_4p_burger_time_deluxe::hack_4p_burger_time_deluxe(std::vector<gb*> g_gb) {

	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));

	game_state = BURGERTIME_TITLE_SCREEN;

	seri_occer = 28885392;
	transfer_speed = 70212;

	init_send_data_queue();
}




void hack_4p_burger_time_deluxe::process() {


	if (game_state != BURGERTIME_CLEAR_SCREEN) {

		//if (v_gb[0]->get_cpu()->total_clock < seri_occer) return;
		//seri_occer = v_gb[0]->get_cpu()->total_clock + transfer_speed;
		if (!is_ready_to_process()) return;

	}

	switch (game_state)
	{
	case BURGERTIME_TITLE_SCREEN:
		break;
	case BURGERTIME_HANDYCAP_SELECTION:
		break;
	case BURGERTIME_START_MATCH:
		break;
	case BURGERTIME_IN_GAME:
		break;
	case BURGERTIME_CLEAR_SCREEN:
		break;
	case BURGERTIME_START_NEXT:
		break;
	default:
		break;
	}

	//seri_occer = v_gb[0]->get_cpu()->total_clock + transfer_speed;
}



/*
void dmg07::serialize(serializer& s)
{

	s_VAR(current_state);
	s_VAR(transfer_speed);
	s_VAR(seri_occer);
	s_VAR(transfer_count);
	s_VAR(phase_byte_count);
	s_VAR(restart_in);
	s_VAR(enter_statu
		s_VAR(packet_size);
	s_VAR(transfer_rate);
	s_VAR(first_aa_trans_nr);
	s_VAR(sync_trans_nr);
	s_VAR(delay);
	s_VAR(ready_to_sync_master);
	s_VAR(master_is_synced);

	s_ARRAY(in_data_buffer);
	s_ARRAY(trans_buffer.data());
	s_ARRAY(answerbytes_buffer.data());
	s_ARRAY(bytes_to_send.data());


}

*/

void hack_4p_burger_time_deluxe::init_send_data_queue() {

	//char start_sequence[11] = { 0xf0, 0xf0, 0xf0, 0xf0, 0xf2, 0xf2, 0xf0, 0xf0,0xf0 ,0xf0, 0xf0 };
	//fill_send_data_queue(start_sequence, 11);

}

void hack_4p_burger_time_deluxe::fill_send_data_queue(char sequence[], int len)
{

	for (int i = 0; i < len; ++i) {
		//send_data_queue.push(sequence[i]);
	}
}

void hack_4p_burger_time_deluxe::get_all_SC_reg_data()
{
	int i = 0;
	for (auto* gb_ptr : v_gb)
	{
		byte data = gb_ptr->get_regs()->SC;
		in_data_buffer[i++] = data;
	}
};

void hack_4p_burger_time_deluxe::get_all_SB_reg_data()
{
	int i = 0;
	for (auto* gb_ptr : v_gb)
	{
		byte data = gb_ptr->get_regs()->SB;
		in_data_buffer[i++] = data;
	}
};

bool hack_4p_burger_time_deluxe::is_expected_data(byte data)
{
	for (size_t i = 0; i < v_gb.size(); i++)
	{
		if (in_data_buffer[i] != data) return false;
	}
	return true;
}

bool hack_4p_burger_time_deluxe::is_ready_to_process() {

	get_all_SC_reg_data();
	return is_expected_data(0x80) && all_IE_are_handled();
}

bool hack_4p_burger_time_deluxe::all_IE_are_handled()
{
	for (size_t i = 0; i < v_gb.size(); i++)
	{
		if ((v_gb[i]->get_regs()->IF & v_gb[i]->get_regs()->IE & INT_SERIAL)) return false;
	}
	return true;

}

void hack_4p_burger_time_deluxe::save_state_mem(void* buf)
{
}

void hack_4p_burger_time_deluxe::restore_state_mem(void* buf)
{
}


void hack_4p_burger_time_deluxe::broadcast_byte(byte data) {}
byte hack_4p_burger_time_deluxe::send_byte(byte which, byte data) { return 0xFF; }

void hack_4p_burger_time_deluxe::serialize(serializer& s){}

void hack_4p_burger_time_deluxe::reset(){}