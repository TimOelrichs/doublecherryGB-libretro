/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Kwirk Multiplayer Hack
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

//-------------------------------------------------

#include "./include/hack_4p_kwirk.hpp"
#include <fstream>


hack_4p_kwirk::hack_4p_kwirk(std::vector<gb*> g_gb)
{
	v_gb = g_gb;
	for (int i = 0; i < v_gb.size(); i++)
	{
		v_gb[i]->set_linked_target(this);
	}
	reset();
	
}

void hack_4p_kwirk::log_traffic(byte id, byte b)
{
	if (!logging_allowed) return;

	std::string filePath = "./kwirkhack_log.txt";
	std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	if (id < 3)
	{
		ofs << "" << std::hex << (int)b << " ";

	}
	else
	{
		ofs << "" << std::hex << (int)b << std::endl;
	}

	ofs.close();

}

byte hack_4p_kwirk::receive_from_linkcable(byte data)
{

	if (master_id == -1)
	{
		get_all_SC_reg_data();

		for (int i = 0; i < v_gb.size(); i++)
		{
			if (in_data_buffer[i] == 0x81) master_id = i;
			log_traffic(i, in_data_buffer[i]);
		}
	}

	get_all_SB_reg_data();

	//check who is master, who send 0x66 
	for (int i = 0; i < v_gb.size(); i++)
	{
		log_traffic(i, in_data_buffer[i]);
	}

	switch (current_state)
	{
	case TITLE_SCREEN:
	{

		bool all_are_ready = true;

		redirect_data(data);
		current_state = SELECT_SCREEN;
		return 0x9F;	

		break;
	}
	case SELECT_SCREEN:
	{
		redirect_data(data);
	
		current_state = SELECT_SKILL;
		return 0xFC;
	
		break;
	}
	case SELECT_SKILL:
	{
		redirect_data(data);
		if (data == 0x08) current_state = SELECT_COURSE_RMS;
		return 0xEE;

		break;
	}
	case SELECT_COURSE_RMS:
	{
		if (data == 0xaa) current_state = SELECT_CONTEST;
		redirect_data(data);
		return 0x0a;

		break;
	}

	case SELECT_CONTEST:
	{

		if (data == 0x08) current_state = SELECT_DISPLAY;
		redirect_data(data);
		return 0xEE;

		break;
	}
	case SELECT_DISPLAY:
	{

		if (data == 0x08) current_state = SELECT_START;
		redirect_data(data);
		return 0xEE;
		break;
	}
	case SELECT_START:
	{
		if (data == 0x08) current_state = INGAME;
		redirect_data(data);
		return 0xEE;
		break; 
	}
	case INGAME:
	{

		byte winner = get_winner_id(data);
		if (winner) {
			current_state = WINNER_SCREEN;
			return handle_winner(winner);
		}
		return send_current_levels();

		break;
	}

	case WINNER_SCREEN:
	{
		handle_loser();
		if (data == 0x71) {
			redirect_data(data);
			return 0x72;
		}
		if (data == 0x72) return 0x71;
		if (data == 0x08) current_state = SELECT_START;
		redirect_data(data);
		return 0xEE;
		break;
	}

	default:
		break;
	}

	
	return 0xFF; 
}


void hack_4p_kwirk::get_all_SB_reg_data()
{
	for (int i = 0; i < v_gb.size(); i++)
	{
		byte data = v_gb[i]->get_regs()->SB;
		in_data_buffer[i] = data;
	}
}

void hack_4p_kwirk::get_all_SC_reg_data()
{
	for (int i = 0; i < v_gb.size(); i++)
	{
		byte data = v_gb[i]->get_regs()->SC;
		in_data_buffer[i] = data;
	}
}

void hack_4p_kwirk::redirect_data(byte data)
{

	for (int i = 0; i < v_gb.size(); i++)
	{
		if (i == master_id)
		{
			log_traffic(i, data);
			continue;	
		}

		in_data_buffer[i] = v_gb[i]->receive_from_linkcable(data);
		log_traffic(i, in_data_buffer[i]);
	

	}
}

byte hack_4p_kwirk::get_max_level()
{
	get_all_SB_reg_data();
	byte max_value = 0;

	for (int i = 0; i < v_gb.size(); i++)
	{
		if (in_data_buffer[i] > max_value) max_value = in_data_buffer[i];
	}
	return max_value;
}

byte hack_4p_kwirk::get_second_level()
{
	byte max = get_max_level();
	byte second = 0;
	for (int i = 0; i < v_gb.size(); i++)
	{
		if (in_data_buffer[i] > second && in_data_buffer[i] < max) second = in_data_buffer[i];
	}
	return second; 
}

bool hack_4p_kwirk::has_draw_max_level()
{
	byte max = get_max_level();
	byte count = 0;
	for (int i = 0; i < v_gb.size(); i++)
	{
		if (in_data_buffer[i] == max) count++;
		if (count >= 2) return true; 
	}
	return false;
}

byte hack_4p_kwirk::send_current_levels() {
	
	byte max = get_max_level();
	bool draw = has_draw_max_level(); 
	byte second = get_second_level();
	byte my_level = 0;

	for (int i = 0; i < v_gb.size(); i++)
	{
		if (i == master_id) continue;

		my_level = in_data_buffer[i];
		if (my_level < max) v_gb[i]->receive_from_linkcable(max);
		else {
			if(draw)
				v_gb[i]->receive_from_linkcable(max);
			else
				v_gb[i]->receive_from_linkcable(second);
		}
	}
	
	my_level = in_data_buffer[master_id];
	if (my_level < max) return max;
	return draw ? max : second; 


}

byte hack_4p_kwirk::get_winner_id(byte data) {
	
	for (int i = 0; i < v_gb.size(); i++)
	{
		if (in_data_buffer[i] == 0x71) return i + 1;
	}
	if (data == 0x71) return (master_id + 1);
	return 0;
}

byte hack_4p_kwirk::handle_winner(int winner_id) {

	if (winner_id == (master_id + 1)) {
		redirect_data(0x71);
	}
	else {

		v_gb[winner_id-1]->receive_from_linkcable(0x72);

		for (int i = 0; i < v_gb.size(); i++)
		{
			if (i == master_id || winner_id == (i+1)) continue;
			v_gb[i]->receive_from_linkcable(0x71);
		}
	}

	return winner_id == (master_id+1) ? 0x01 : 0x71;
}

void hack_4p_kwirk::handle_loser() {

	get_all_SB_reg_data();

	for (int i = 0; i < v_gb.size(); i++)
	{
			v_gb[i]->receive_from_linkcable(0x71);
	}

}


void hack_4p_kwirk::reset()
{
	master_id = -1;
	current_state = TITLE_SCREEN;

	for (int i = 0; i < 16; i++)
	{
		in_data_buffer[i] = 0;
	}

}