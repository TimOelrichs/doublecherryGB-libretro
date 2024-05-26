/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
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


#include "dmg07.hpp"
#include "serializer.h"
#include <iostream>
#include <vector>
#include <queue>
#include <string>


dmg07::dmg07(std::vector<gb*> g_gb)  {

	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));

	for (size_t i = 0; i < 4; i++)
	{
		trans_buffer[i] = std::vector<byte>();
		ans_buffer[i] = std::vector<byte>();
	}
	reset();
}



void dmg07::reset() 
{
	current_state = PING_PHASE;
	transfer_count = 0;
	phase_byte_count = 0;

	restart_in = 0;
	enter_status = 0x00;

	packet_size = 0;
	transfer_rate = 0x00;
	transfer_speed = 512 * 8;
	seri_occer = 512 * 8 * 5;

	first_aa_trans_nr = 0;
	sync_trans_nr = 0;

	delay = 0;
	//buffer_start_point = 0;

	//process_counter = 0;
	//process_occer = 4;

	ready_to_sync_master = false;
	master_is_synced = false;
	//ready_to_sync_others = false;
	//others_are_synced = false;

	bytes_to_send = std::vector<byte>();

	for (byte i = 0; i < dmg07::v_gb.size(); i++)
	{
		in_data_buffer[i] = 0;
		trans_buffer[i].clear();
		ans_buffer[i].clear();
		//last_trans_nr[i] = 0;
		//last_trans_nr[i] = 0;
	}
}

/*
void dmg07::log_traffic(byte id, byte b)
{
	if (logging_allowed)
	{
		std::string filePath = "./dmg07_log.txt";
		std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
		if (id < v_gb.size())
		{
			ofs << "" << std::hex << (int)b << " ";

		}
		else
		{
			ofs << "" << std::hex << (int)b << std::endl;
		}

		ofs.close();
	}

}
*/

void dmg07::log_save_state(char* data, size_t size)
{
	//if (logging_allowed)
	{
		{
			std::string filePath = "./dmg07_savesate_log.bin";
			std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

			for (int i = 0; i < size; i++)
			{
				ofs << data[i];
			}
			ofs.close();
		}
	}
}

void dmg07::restart_pingphase()
{
	current_state = PING_PHASE;
	transfer_count = 0;

	enter_status = 0x00;
	packet_size = 0;
	transfer_rate = 0x00;
	//transfer_speed = 70220 / 2;
	//seri_occer = 2042 * 16;
	phase_byte_count = 0;
	delay = 0;
	//ready_to_sync_others = false; 

	bytes_to_send = std::vector<byte>();

	for (byte i = 0; i < dmg07::v_gb.size(); i++)
	{
		trans_buffer[i].clear();
		ans_buffer[i].clear();
		//last_trans_nr[i] = 0;
	}
}

void dmg07::handle_answer(int i, byte dat)
{
	switch (current_state)
	{
	case PING_PHASE:
	{
	
		switch (ans_buffer[i].size())
		{
		case 0:
		{
			if (dat == 0x88)
			{
				ans_buffer[i].push_back(dat);
				//last_trans_nr[i] = transfer_count;
			}

			if (dat == 0xAA && i == 0) //&& ((transfer_count % 4) == 0) )
			{
				ans_buffer[i].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

				break;

			}
			break;
		}
		case 1:
		{
			if (dat == ans_buffer[i].at(0)) //&& //last_trans_nr[i] == (transfer_count - 1))
			{
				ans_buffer[i].push_back(dat);
				//last_trans_nr[i] = transfer_count;

				if (dat == 0x88)
				{
					enter_status |= (byte)((byte)1 << (byte)(i + 4));
				}

			}
			else if (dat == 0xAA && i == 0)//&& ((transfer_count % 4) == 0))
			{
				ans_buffer[i].clear();
				ans_buffer[i].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

			}
			else ans_buffer[i].clear();


			break;
		}
		case 2:
		{

			if (dat == 0xAA && ans_buffer[i].at(0) == 0xAA && ans_buffer[i].at(1) == 0xAA && i == 0) //&& //last_trans_nr[i] == (transfer_count - 1))
			{
				current_state = SYNC_PHASE;
				ans_buffer[i].push_back(dat);
				break;


			}
			else if (ans_buffer[i].at(0) == 0x88 && ans_buffer[i].at(1) == 0x88)
			{
				ans_buffer[i].push_back(dat);
				//last_trans_nr[i] = transfer_count;
				
				if (!transfer_rate)
				{
					transfer_rate = dat;
					
					/* Hack to get my assumed ping speedrate*/
					if (dat == 0xF0)  // Janshiro Games
						transfer_speed = 5928 * 3;
					else if (dat == 0xA0)	// Jinsei Game Densetsz
						transfer_speed = 5928 * 4;
					else if (dat == 0x85) // Top Rank Tennis 
						transfer_speed = 512 * 8;
					else if (dat == 0xf3) transfer_speed = 70216;
					else {
						char ping_speed_multiplayer = dat & 0x0F;
						if (ping_speed_multiplayer)
						{
							if (ping_speed_multiplayer == 1 || ping_speed_multiplayer == 0)
								transfer_speed = 512 * 16;
							else
								transfer_speed = 512 * ping_speed_multiplayer;
						}
					}
						
				
					
					/*
					byte ping_rate = (byte)(transfer_rate & 0x0F);
					if(ping_rate)	transfer_speed = 2048 * (ping_rate); // PING SPEED
					*/
					
				}
			}
			else if (dat == 0xAA && i == 0)//&& ((transfer_count % 4) == 0))
			{
				ans_buffer[i].clear();
				ans_buffer[i].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

			}
			else ans_buffer[i].clear();

			break;
		}
		case 3:
		{
			if ((dat == 0xAA || dat == 0x00) && ans_buffer[i].at(0) == 0xAA && ans_buffer[i].at(1) == 0xAA && ans_buffer[i].at(2) == 0xAA && i == 0) //&& //last_trans_nr[i] == (transfer_count - 1))
			{

				current_state = SYNC_PHASE;
				ans_buffer[i].clear();


			}
			else if (ans_buffer[i].at(0) == 0x88 && ans_buffer[i].at(1) == 0x88)
			{
				if (!packet_size)
				{
					packet_size = dat;
					
				}
			}
			else if (dat == 0xAA && i == 0)
			{
				ans_buffer[i].clear();
				ans_buffer[i].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

			}

			ans_buffer[i].clear();

			break;
		}
		}
		break;
	}
	case SYNC_PHASE:
		/*
		if (i == 0 && dat != 0xAA)
		{
			if (transfer_count % 4 == 0) return;
			//master_is_synced = true; 
			//sync_trans_nr = transfer_count;
			return;
		}
		if (i != 0 && dat == 0x00)
		{
			//if (transfer_count % 4 == 0) return;
			//others_are_synced = true;
			return;
		}
		*/
		break;
	case TRANSMISSION_PHASE:
	{
		if (i == 0)
		{
			switch (ans_buffer[i].size())
			{
			case 0:
			{
				if (dat == 0xFF && transfer_count % 4 == 0)
				{
					ans_buffer[i].push_back(dat);
					//last_trans_nr[i] = transfer_count;
				}
				break;
			}
			case 1:
			{
				if (dat == 0xFF)
				{

					ans_buffer[i].clear();
					bytes_to_send = std::vector<byte>();
					restart_in = packet_size * 4;

					for (int i = 0; i < (packet_size * 4); i++)
					{
						bytes_to_send.push_back(0xFF);
					}
					restart_in += transfer_count;
					break;
				}
				else
				{
					ans_buffer[i].clear();
					break;
				}
				break;
			}

			}

		}
		break;
	}
	}
}



byte dmg07::send_byte(byte which, byte dat)
{
	byte ret = v_gb[which]->get_cpu()->seri_send(dat);

	if (which == 0) log_traffic(0, dat);
	log_traffic(which + 1, ret);

	handle_answer(which, ret);

	return ret; 
}

/*
void dmg07::broadcast_byte(byte dat)
{
	for (byte i = 0x00; i < (byte)dmg07::v_gb.size(); i++)
	{
		send_byte(i, dat);
	}
}

*/

void dmg07::send_sync_bytes()
{
	
	if (!ready_to_sync_master)
	{

		send_each_ping_byte();
		
		if (transfer_count % 4 == 3)
			ready_to_sync_master = true;

		return;
	}
	
	if (!master_is_synced)
	{	
		if (transfer_rate != 0xA0 && transfer_rate != 0x85)	// Jinsei Game Densetsi and Top Rank Tennis
			transfer_speed = 70216;

		broadcast_byte(0xCC);
		master_is_synced = transfer_count % 4 == 3;
		return;
	}

	if (++phase_byte_count >= (packet_size * 4) && transfer_count % 4 == 0)
	{
		broadcast_byte(0x00);


		/*
		* This is just a hack, to match my assumend speed rate.
		* i didn't want to hardcode for every game. 
		* haven't found out the meaning of the magic-byte "transfer_rate" yet
		
		00 = 5928
		10 = 5928 * 2
		11 = 5928 * 4
		*/
	
		if (transfer_rate == 0xF0)  //Janshiro Games
			transfer_speed = 6144 * 3;
		else if (transfer_rate == 0xA0)	// Jinsei Game Densetsi
			transfer_speed = 6144 * 4;
		else {
			byte speed_multiplier = ((transfer_rate & 0x80) >> 7) | ((transfer_rate & 0x40) >> 5);
			transfer_speed = 6144 * ((int)speed_multiplier + 1);
		}
	
		/*
		v_gb[0]->get_regs()->SC = transfer_rate;
		v_gb[0]->get_cpu()->seri_occer = seri_occer = 0x7fffffff;
		v_gb[0]->set_target(v_gb[1]);
		*/

		current_state = TRANSMISSION_PHASE;
		phase_byte_count = 0;
		delay = 0;
		clear_all_buffers();
		return;
	}

}

byte dmg07::send_ping_byte(byte which)
{
	byte dat = (transfer_count % 4) ? (enter_status | (byte)(which + 1)) : 0xFE;
	return send_byte(which, dat);
}

void dmg07::send_each_ping_byte()
{
	for (byte i = 0x00; i < (byte)dmg07::v_gb.size(); i++)
		send_ping_byte(i);

	
}

void dmg07::clear_all_buffers() {
	for (byte i = 0; i < dmg07::v_gb.size(); i++) trans_buffer[i].clear();
}


void dmg07::fill_buffer_for_less_than_4p() 
{
	if (v_gb.size() < 4)
	{
		for (int i = 0; i < 4 - v_gb.size(); i++)
		{
			for (int i = 0; i < packet_size; i++)
			{
				bytes_to_send.push_back(0);
			}
			
		}
	}
}

void dmg07::process()
{

	if (!is_ready_for_next_tik() || !all_IE_are_handled()) return;
	//if (!is_ready_for_next_tik()) return;
	update_seri_occer();

	switch (current_state)
	{
		case PING_PHASE: send_each_ping_byte(); break;
		case SYNC_PHASE: send_sync_bytes(); break;
		case TRANSMISSION_PHASE:
		{

			// start buffering data
			if (bytes_to_send.empty())	
			{
				//Get Packets
				for (byte i = 0; i < v_gb.size(); i++)
				{
					byte ret = v_gb[i]->get_cpu()->seri_send(0x00);
					//handle_answer(i, ret);

					if (i == 0)log_traffic(0, 0x00);
					log_traffic(i + 1, ret);

					if (trans_buffer[i].size() < packet_size)
					{
						trans_buffer[i].push_back(ret);
	
					}

				}
				// if ready fill queue bytes_to_send
				//if (++delay == ((packet_size * 4) ))
				if (++delay == ((packet_size * 4) - 1))
				{
					for (byte i = 0; i < v_gb.size(); i++)
					{
						for (const auto& e : trans_buffer[i])
							bytes_to_send.push_back(e);

						trans_buffer[i].clear();
					}
					delay = 0;
					// buffer with zeros for less than 4 players
					fill_buffer_for_less_than_4p();
				}
				
				

			}

			//if queue ready send from queue and buffer new data
			else
			{
					//send packets and get new packets

					byte next_byte = bytes_to_send.front();
					bytes_to_send.erase(bytes_to_send.begin());

					for (byte i = 0; i < v_gb.size(); i++)
					{
						byte ret = v_gb[i]->get_cpu()->seri_send(next_byte);

						if (i == 0)log_traffic(0, next_byte);
						log_traffic(i + 1, ret);

						if (trans_buffer[i].size() < packet_size && delay > 0)
						{
							trans_buffer[i].push_back(ret);
						}
					}
					delay++;

					//refill queue
					if (bytes_to_send.empty())
					{
						if (transfer_count == restart_in)
						{
							restart_pingphase();
							return;
						}

						for (byte i = 0; i < v_gb.size(); i++)
						{
							
							for (const auto& e : trans_buffer[i])
								bytes_to_send.push_back(e); // create bytes_to_send queue

							trans_buffer[i].clear();
							
						}
						delay = 0;
						fill_buffer_for_less_than_4p();
					}


				}

				break;
			}
	}

	transfer_count++;
	

}


/* netplay support functions*/
	

size_t dmg07::get_state_size(void)
{
	size_t ret = 0;
	serializer s(&ret, serializer::COUNT);

	int size;
	for (size_t i = 0; i < 4; i++)
	{
		size = trans_buffer[i].size();
		s_VAR(size);
		s_ARRAY(trans_buffer[i].data());
		size = ans_buffer[i].size();
		s_VAR(size);
		s_ARRAY(ans_buffer[i].data());
	}

	size = bytes_to_send.size();
	s_VAR(size);
	s_ARRAY(bytes_to_send.data());


	serialize(s);
	return ret;
}


void dmg07::save_state_mem(void* buf)
{
	serializer s(buf, serializer::SAVE_BUF);

	int size;
	for (size_t i = 0; i < 4; i++)
	{
		size = trans_buffer[i].size();
		s_VAR(size);
		if(size) s_ARRAY(trans_buffer[i].data());
		size = ans_buffer[i].size();
		s_VAR(size);
		if (size) s_ARRAY(ans_buffer[i].data());
	}
	size = bytes_to_send.size();
	s_VAR(size);
	if (size) s_ARRAY(bytes_to_send.data());

	serialize(s);
}

void dmg07::restore_state_mem(void* buf)
{
	serializer s(buf, serializer::LOAD_BUF);

	int size;
	byte* tmp;
	
	for (int i = 0; i < 4; i++)
	{
		trans_buffer[i] = std::vector<byte>();
		ans_buffer[i] = std::vector<byte>();

		s_VAR(size);
		if (size) {
			tmp = new byte[size];
			s_ARRAY(tmp);

			for (int j = 0; j < size; j++)
			{
				trans_buffer[i].push_back(tmp[j]);
			}
		}
		
		s_VAR(size);
		if (size) {
			tmp = new byte[size];
			s_ARRAY(tmp);


			for (int k = 0; k < size; k++)
			{
				ans_buffer[i].push_back(tmp[k]);
			};
		}
	
	}

	bytes_to_send = std::vector<byte>();
	s_VAR(size);
	if (size) {
		tmp = new byte[size];
		s_ARRAY(tmp);

		for (size_t i = 0; i < size; i++)
		{
			bytes_to_send.push_back(tmp[i]);
		};
	}
	
	//delete tmp; 
	serialize(s);
}



void dmg07::serialize(serializer& s) 
{
	
	s_VAR(current_state); 
	s_VAR(transfer_speed);
	s_VAR(seri_occer);
	s_VAR(transfer_count);
	s_VAR(phase_byte_count);
	s_VAR(restart_in);
	s_VAR(enter_status);
	s_VAR(packet_size);
	s_VAR(transfer_rate);
	s_VAR(first_aa_trans_nr);
	s_VAR(sync_trans_nr);
	s_VAR(delay);
	s_VAR(ready_to_sync_master);
	s_VAR(master_is_synced);
	s_ARRAY(in_data_buffer);

}