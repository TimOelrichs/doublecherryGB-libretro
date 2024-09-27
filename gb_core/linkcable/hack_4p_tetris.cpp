/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Tetris Multiplayer Hack
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

#include "./include/hack_4p_tetris.hpp"
#include "../../libretro/libretro.h"
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <fstream>

extern bool logging_allowed; 
extern int emulated_gbs;
extern retro_environment_t environ_cb;
extern unsigned libretro_msg_interface_version;

hack_4p_tetris::hack_4p_tetris(std::vector<gb*> g_gb) {

	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));

	tetris_state = TITLE_SCREEN;

	seri_occer = 4096 * 1024 * 4;
	transfer_speed = 4096 * 1024 / 16;

	for (size_t i = 0; i < emulated_gbs; i++)
	{
		players_state[i] = IS_ALIVE;
		//cpu* cpu = v_gb[i]->get_cpu();
	}
		
	init_send_data_vec();

	tetris_state = TITLE_SCREEN;

	current_max_height = 0;
	lines_from_player_id = 0;
	process_counter = 0;
	//process_occer = 5;

}

void hack_4p_tetris::clear_data_for_next_round()
{
	for (size_t i = 0; i < emulated_gbs; i++)
	{
		players_state[i] = IS_ALIVE;
		//cpu* cpu = v_gb[i]->get_cpu();
		in_data_buffer[i] = 0;
		next_bytes_to_send[i] = 0;
	}
}

void hack_4p_tetris::reset()
{

	tetris_state = TITLE_SCREEN;

	seri_occer = 4096 * 1024 * 4;
	transfer_speed = 4096 * 1024 / 16;

	current_max_height = 0;
	lines_from_player_id = 0;
	process_counter = 0;
	//process_occer = 5;

	clear_data_for_next_round();
	send_data_vec = std::vector<byte>();
	init_send_data_vec();

	for (int i = 0; i < 16; i++)
	{
		players_state[i] = IS_ALIVE;
	
		lines_to_send[i] = 0;
		next_bytes_to_send[i] = 0;
		win_counter[i] = 0;
	}
}

void hack_4p_tetris::hard_reset() {

	for (size_t i = 0; i < emulated_gbs; i++)
	{
		v_gb[i]->reset();
	}
	tetris_state = TITLE_SCREEN;

	seri_occer = 4096 * 1024 * 4;
	transfer_speed = 4096 * 1024 / 16;

	for (int i = 0; i < 16; i++)
	{
		players_state[i] = IS_ALIVE;

		lines_to_send[i] = 0;
		next_bytes_to_send[i] = 0;
		win_counter[i] = 0;
	}

	for (size_t i = 0; i < emulated_gbs; i++)
	{
		players_state[i] = IS_ALIVE;
		//cpu* cpu = v_gb[i]->get_cpu();
	}

	init_send_data_vec();

	tetris_state = TITLE_SCREEN;

	current_max_height = 0;
	lines_from_player_id = 0;
	process_counter = 0;



}

void hack_4p_tetris::init_send_data_vec() {

	//send_data_vec.push_back(0x29);   //start connection
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x1c);   //music a is selected 
	send_data_vec.push_back(0x50);   //confirm music selection
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x0);    //height 0 is selected
	send_data_vec.push_back(0x60);   //confirm height selection
	send_data_vec.push_back(0x29);   //start sending height blocks
}

void hack_4p_tetris::log_traffic(byte id, byte b)
{
	if (!logging_allowed) return; 

	std::string filePath = "./tetrishack_log.txt";
	std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	if (id < 4)
	{
		ofs << "" << std::hex << (int)b << " ";

	}
	else
	{
		ofs << "" << std::hex << (int)b << std::endl;
	}

	ofs.close();

}

/*
bool hack_4p_tetris::is_ready_to_process() {

	get_all_SC_reg_data();
	return is_expected_data(0x80) && all_IE_are_handled();
}

bool hack_4p_tetris::all_IE_are_handled() 
{
	for (int i = 0; i < emulated_gbs; i++)
	{
		if ((v_gb[i]->get_regs()->IF & v_gb[i]->get_regs()->IE & INT_SERIAL)) return false;
	}
	return true;

}
*/

void hack_4p_tetris::process() {


	if (tetris_state != WINNER_SCREEN) {

		if (!is_ready_for_next_tik() || !gbs_are_ready_to_process()) return;
		update_seri_occer();
		//seri_occer = v_gb[0]->get_cpu()->total_clock + transfer_speed;
	
	}
	
	switch (tetris_state)
	{
	case TITLE_SCREEN:
	{
		
		get_all_SB_reg_data();
		if (is_expected_data(0xFF)) return; 
		get_all_SB_reg_data();
		if (is_expected_data(0x55))
		{
			broadcast_byte(0x29);
			tetris_state = MUSIC_SELECT;
		}
		
	
		/*
		if (++process_counter < 5) return 0;
		process_counter = 0;
		if (!is_ready_to_process()) return 0;
		*/
		//if (!is_ready_to_process()) return;

			
		
		break;
	}
	case MUSIC_SELECT:
	{
		/*
		if (++process_counter < 5) return 0;
		process_counter = 0;
		*/
		//if (!is_ready_to_process()) return;

		if (!send_data_vec.empty())
		{
			byte data = send_data_vec.front();
			send_data_vec.erase(send_data_vec.begin());
			broadcast_byte(data);
			if (send_data_vec.empty())
			{
				tetris_state = HEIGHT_SELECT;
				generate_height_blocks();
			}
	
		}
		break;
	}
	case HEIGHT_SELECT:
	{
		/*
		if (++process_counter < 5 && !is_ready_to_process()) return 0;
		process_counter = 0;
		*/
		transfer_speed = 4096;
		
		if (!out_height_blocks.empty())
		{
			byte data = out_height_blocks.front();
			out_height_blocks.erase(out_height_blocks.begin());

			broadcast_byte(data);
			if (out_height_blocks.empty())
			{
				generate_falling_blocks();
			}
			break;
		}
		if (!out_falling_blocks.empty())
		{
			byte data = out_falling_blocks.front();
			out_falling_blocks.erase(out_falling_blocks.begin());

			broadcast_byte(data);
			if (out_falling_blocks.empty())
			{
				tetris_state = IN_GAME;
			}
			break;
		}
		break;
	}
	case IN_GAME:
	{

		transfer_speed = 4096 * 1024 / 8;
		/*
		if (++process_counter < 5) return 0;
		process_counter = 0;
		if (!is_ready_to_process()) return 0;
		*/


		get_all_SC_reg_data();
		handle_ingame_data();
		break;

	}
	case WINNER_SCREEN:
	{
		
		//wait 5 Seconds
		if (++process_counter < 154 * 60 * 5) return;
		process_counter = 0;
	
		if (winner_winner_chicken_dinner()) {
			hard_reset();
			break; 
		}
		// would be better to handle actually answers
		send_data_vec.push_back(0x60);   
		send_data_vec.push_back(0x02);   
		send_data_vec.push_back(0x02);   
		send_data_vec.push_back(0x02);
		send_data_vec.push_back(0x79);    
		//send_data_vec.push_back(0x29);   //start sending height blocks

		clear_data_for_next_round();
		tetris_state = START_NEXT;

		break;
	}
	case START_NEXT:
	{
		/*
		if (++process_counter < 5 && !is_ready_to_process()) return 0;
		process_counter = 0;
		*/

		if (!send_data_vec.empty())
		{
			byte data = send_data_vec.front();
			send_data_vec.erase(send_data_vec.begin());
			broadcast_byte(data);
		
			break; 
		}

		
		get_all_SB_reg_data();

		if (is_expected_data(0x55))
		{
			
			tetris_state = HEIGHT_SELECT;
			generate_height_blocks();
	
		}
		broadcast_byte(0x29);

		break;
	}

	default:
		break;
	}


	//seri_occer = v_gb[0]->get_cpu()->total_clock + transfer_speed;
	
}


void hack_4p_tetris::generate_height_blocks() 
{
	//for now create just empty blocks
	for (int i = 0; i < 100; i++)
	{
		out_height_blocks.push_back(0x2f); // 0x2f empty block
	}
	out_height_blocks.push_back(0x29); // start sending fallingblocks
};

void hack_4p_tetris::generate_falling_blocks()
{
	std::srand(std::time(0));
	int size_of_choice = sizeof(falling_block_choice) / sizeof(byte);

	for (int i = 0; i < 256; ++i) 
	{
		int randomIndex = std::rand() % size_of_choice;
		out_falling_blocks.push_back(falling_block_choice[randomIndex]);
	}
	out_falling_blocks.push_back(0x30);    //finished sending blocks
	out_falling_blocks.push_back(0x00);
	out_falling_blocks.push_back(0x02);
	out_falling_blocks.push_back(0x02);
	out_falling_blocks.push_back(0x20);   //start in_game
	out_falling_blocks.push_back(0x0);
	out_falling_blocks.push_back(0x0);
	out_falling_blocks.push_back(0x0);
	out_falling_blocks.push_back(0x0);
	
};


/*
void hack_4p_tetris::get_all_SC_reg_data()
{
	for (int i = 0; i < emulated_gbs; i++)
	{
		byte data = v_gb[i]->get_regs()->SC;
		in_data_buffer[i] = data;
	}
};

void hack_4p_tetris::get_all_SB_reg_data()
{
	for (int i = 0; i < emulated_gbs; i++)
	{
		byte data = v_gb[i]->get_regs()->SB;
		in_data_buffer[i] = data;
	}
};

bool hack_4p_tetris::is_expected_data(byte data)
{
	for (int i = 0; i < emulated_gbs; i++)
	{
		if (in_data_buffer[i] != data) return false; 
	}
	return true;
}

*/

void hack_4p_tetris::handle_ingame_data() 
{
	get_all_SB_reg_data();
	update_ingame_states();
	send_ingame_bytes();
}

/*
void hack_4p_tetris::send_byte(byte which, byte dat)
{
	byte ret = v_gb[which]->get_cpu()->seri_send(dat);

	if (which == 0) log_traffic(0, dat);
	log_traffic(which + 1, ret);

}

void hack_4p_tetris::broadcast_byte(byte dat)
{
	for (byte i = 0x00; i < (byte)emulated_gbs; i++)
	{
		send_byte(i, dat);
	}
}
*/

int hack_4p_tetris::player_alive_count() {
	int alive_count = 0;

	for (int i = 0; i < emulated_gbs; i++)
	{
		if (players_state[i] == IS_ALIVE || players_state[i] == IS_SENDING_LINES)
		{
			alive_count++;
		}
	}
	return alive_count; 
}

int hack_4p_tetris::check_winner_id()
{
	int alive_count = 0;
	int last_alive_id = 0;

	for (int i = 0; i < emulated_gbs; i++)
	{
		if (players_state[i] == IS_ALIVE)
		{
			alive_count++;
			last_alive_id = i + 1;
		}
	}

	if (alive_count == 1) return last_alive_id;
	if (alive_count == 0) return -1;
	return 0;
}

bool hack_4p_tetris::winner_winner_chicken_dinner() {

	for (int i = 0; i < emulated_gbs; i++)
	{
		if (win_counter[i] == 4) return true; 
	}
	return false; 
}

void hack_4p_tetris::update_ingame_states()
{
	current_max_height = 0;

	for (int i = 0; i < emulated_gbs; i++)
	{
		switch (players_state[i])
		{
		case IS_WINNER:
		{

			if (in_data_buffer[i] == 0x34)
			{
				players_state[i] = IS_LOOSER; // ;D need new states
				win_counter[i]++;
				next_bytes_to_send[i] = 0x43;

				std::ostringstream oss;
				oss << (i + 1);
				std::string msg_str = "congrats to player " + oss.str() + "!";

				if (libretro_msg_interface_version >= 1)
				{	
					struct retro_message_ext msg = {
					   msg_str.data(),
					   1000,
					   1,
					   RETRO_LOG_INFO,
					   RETRO_MESSAGE_TARGET_OSD,
					   RETRO_MESSAGE_TYPE_NOTIFICATION,
					   -1
					};
					environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE_EXT, &msg);
				}
				else
				{
					struct retro_message msg = {
						msg_str.data(),
					   120
					};
					environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
				}
			}
			else {
				next_bytes_to_send[i] = 0x34;

			}
			break; 
		}
		case IS_LOOSER:
		{
			players_state[i] = IS_IN_WINNER_SCREEN;
			break; 
		}
		case IS_KO:
		{
			if (in_data_buffer[i] == 0x34)
			{
				players_state[i] = IS_LOOSER;
				next_bytes_to_send[i] = 0x43;
			}
			else {
				next_bytes_to_send[i] = 0xaa;

			}
			break;
		}
		case IS_ALIVE:
		{

			if (in_data_buffer[i] == 0x44) // not ready yet
			{
				next_bytes_to_send[i] = 0x20;
				break; 
			}

			if (in_data_buffer[i] == 0xaa) // 0xaa is ko
			{
				players_state[i] = IS_KO;
				next_bytes_to_send[i] = 0xaa; // send ko for draw, cause only winning counts

				std::ostringstream oss;
				oss << (i + 1);
				std::string msg_str = "Player " + oss.str() + " KO! ";
				oss.str("");
				oss << player_alive_count();
				msg_str = msg_str + oss.str() + " players left";


				if (libretro_msg_interface_version >= 1)
				{
					
					struct retro_message_ext msg = {
					   msg_str.data(),
					   1000,
					   1,
					   RETRO_LOG_INFO,
					   RETRO_MESSAGE_TARGET_OSD,
					   RETRO_MESSAGE_TYPE_NOTIFICATION,
					   -1
					};
					environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE_EXT, &msg);
				}
				else
				{
					struct retro_message msg = {
						msg_str.data(),
					   120
					};
					environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
				}

				break;
			}

			if (in_data_buffer[i] == 0x34)
			{
				players_state[i] = IS_WINNER;
				next_bytes_to_send[i] = 0x43;
				break;
			}


			if ((in_data_buffer[i] & (byte)0x80) == (byte)0x80) // 0x8x send lines
			{
				if (in_data_buffer[i] != (byte)0x80) {
					hack_4p_tetris_lines_packet lines;
					lines.lines = in_data_buffer[i];
					lines.from = i;
					lines_vec.push_back(lines);
				}
				break; 
			}

			current_max_height = current_max_height < in_data_buffer[i] ? in_data_buffer[i] : current_max_height;
			break; 
		}

		default:
			break;
		}
	}

	if (all_are_in_winnerscreen()) tetris_state = WINNER_SCREEN;

}

bool hack_4p_tetris::all_are_in_winnerscreen() {

	for (int i = 0; i < emulated_gbs; i++)
	{
		if (players_state[i] != IS_IN_WINNER_SCREEN) return false;
	}
	return true; 
}

void hack_4p_tetris::send_ingame_bytes() 
{
	int winner = check_winner_id();
	if (winner && winner != -1) {
		next_bytes_to_send[winner - 1] = 0xaa;
		players_state[winner - 1] = IS_WINNER;
	}
	

	if (!lines_vec.empty())
	{
		hack_4p_tetris_lines_packet next = lines_vec.front();
		lines_vec.erase(lines_vec.begin());

		//choose random players id to get lines
		int players_alive = player_alive_count();
		int quarter_player_alive = (int)(players_alive / 4) ? (int)(players_alive / 4) : 1;
		int players_got_lines = 0;
		bool done = players_got_lines >= quarter_player_alive;
		int max_trys = emulated_gbs * 4;
		int current_try = 0;

		std::srand(std::time(0));
		while (!done) {

				if (current_try++ >= max_trys) done = true;
				
				int randomIndex = std::rand() % emulated_gbs;
		
				if (players_state[randomIndex] == IS_IN_WINNER_SCREEN)  continue;
				if (randomIndex == next.from) continue;
				if (next_bytes_to_send[randomIndex]) continue;

				next_bytes_to_send[randomIndex] = next.lines;
				done = ++players_got_lines >= quarter_player_alive;
				
		}
	
		/*
		for (int i = 0; i < emulated_gbs; i++)
		{
			if (!((players_state[i] == IS_ALIVE) || (players_state[i] == IS_SENDING_LINES)))  continue;
			if (next.from == i) continue;
			if (!next_bytes_to_send[i]) next_bytes_to_send[i] = next.lines; // ?? do lines get lost??
		}*/


	}

	for (int i = 0; i < emulated_gbs; i++)
	{
		if (!next_bytes_to_send[i]) next_bytes_to_send[i] = current_max_height;
		if (players_state[i] != IS_IN_WINNER_SCREEN) 	send_byte(i, next_bytes_to_send[i]);
	
		next_bytes_to_send[i] = 0;
	}

}




/* netplay support functions*/


size_t hack_4p_tetris::get_state_size(void)
{
	size_t ret = 0;
	serializer s(&ret, serializer::COUNT);
	serialize(s);

	int size;
	size = out_height_blocks.size();
	s_VAR(size);
	s_ARRAY(out_height_blocks.data());

	size = out_falling_blocks.size();
	s_VAR(size);
	s_ARRAY(out_falling_blocks.data());

	size = send_data_vec.size();
	s_VAR(size);
	s_ARRAY(send_data_vec.data());

	size = lines_vec.size();
	s_VAR(size);
	s_ARRAY(lines_vec.data());

	return ret;
}



void hack_4p_tetris::save_state_mem(void* buf)
{
	serializer s(buf, serializer::SAVE_BUF);
	serialize(s);

	int size;

	size = out_height_blocks.size();
	s_VAR(size);
	if(size)s_ARRAY(out_height_blocks.data());

	size = out_falling_blocks.size();
	s_VAR(size);
	if (size)s_ARRAY(out_falling_blocks.data());
	
	size = send_data_vec.size();
	s_VAR(size);
	if (size)s_ARRAY(send_data_vec.data());

	size = lines_vec.size();
	s_VAR(size);
	if (size)s_ARRAY(lines_vec.data());

	
}

void hack_4p_tetris::restore_state_mem(void* buf)
{
	serializer s(buf, serializer::LOAD_BUF);
	serialize(s);

	int size;
	byte* tmp;
	out_height_blocks = std::vector<byte>();
	out_falling_blocks = std::vector<byte>();
	send_data_vec = std::vector<byte>();
	lines_vec = std::vector<hack_4p_tetris_lines_packet>();

	s_VAR(size);
	if (size) {
		tmp = new byte[size];
		s_ARRAY(tmp);

		for (int i = 0; i < size; i++)
			out_height_blocks.push_back(tmp[i]);
	}
	
	s_VAR(size);
	if (size) {
		tmp = new byte[size];
		s_ARRAY(tmp);
		for (int i = 0; i < size; i++)
			out_falling_blocks.push_back(tmp[i]);
	}

	s_VAR(size);
	if (size) {
		tmp = new byte[size];
		s_ARRAY(tmp);

		for (int i = 0; i < size; i++)
			send_data_vec.push_back(tmp[i]);
	}


	s_VAR(size);
	if (size) {
		hack_4p_tetris_lines_packet* lp = new hack_4p_tetris_lines_packet[size];
		s_ARRAY(lp);

		for (int i = 0; i < size; i++)
			lines_vec.push_back(lp[i]);
	}
	
}


void hack_4p_tetris::serialize(serializer& s)
{

	s_ARRAY(in_data_buffer);
	s_ARRAY(falling_block_choice);
	s_VAR(tetris_state);
	s_VAR(players_state);
	s_VAR(current_max_height);
	s_VAR(lines_from_player_id);
	s_ARRAY(lines_to_send);
	s_VAR(next_bytes_to_send);
	s_VAR(process_counter);
	//s_VAR(process_occer);


}
