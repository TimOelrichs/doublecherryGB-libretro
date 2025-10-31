/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Four Player Adapter (DMG-07) Emulation
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


#include "./include/dmg07.hpp"
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
		answerbytes_buffer[i] = std::vector<byte>();
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

	packet_size = 1;
	transfer_rate = 0x00;
	//transfer_speed = 512 * 8;
	seri_occer = 512 * 8 * 5;

	first_aa_trans_nr = 0;
	sync_trans_nr = 0;

	delay = 0;
	received_pingphase_restart_demand = false; 
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
		answerbytes_buffer[i].clear();
		//last_trans_nr[fromGbId] = 0;
		//last_trans_nr[fromGbId] = 0;
	}
}


void dmg07::log_traffic(byte id, byte b)
{

	//if (logging_allowed)
	if(false)
	{
		std::string filePath = "./dmg07_log.txt";
		std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

		if (!id) {
			ofs << v_gb[0]->get_cpu()->get_clock()-last_log_clock << "\t";
			last_log_clock = v_gb[0]->get_cpu()->get_clock();
		}

		
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


void dmg07::update_seri_occer()
{

	uint32_t now = this->v_gb[0]->get_cpu()->get_clock();

	uint32_t delay = transfer_time_per_byte_in_clocks +
		((transfer_count % (current_packet_size * 4)) ? delay_between_bytes_in_clocks : delay_between_packages_in_clocks);

	this->seri_occer = now + delay;
}

void dmg07::restart_pingphase()
{

	current_state = PING_PHASE;
	transfer_count = 0;
	restart_in = -1;

	enter_status = 0x00;
	packet_size = 0;
	transfer_rate = 0x00;

	phase_byte_count = 0;
	delay = 0;

	int delay_between_bytes_in_clocks = delay_between_bytes_in_clocks_default;
	int delay_between_packages_in_clocks = delay_between_packages_in_clocks_default;

	bytes_to_send = std::vector<byte>();

	for (byte i = 0; i < dmg07::v_gb.size(); i++)
	{
		trans_buffer[i].clear();
		answerbytes_buffer[i].clear();
		
	}
}


void dmg07::handle_answer(int fromGbId, byte data) {
	switch (current_state)
	{
	case PING_PHASE: handle_answer_pingphase(fromGbId, data);
		break;
	case SYNC_PHASE:
		break;
	case TRANSMISSION_PHASE: handle_answer_for_restart_pingphase(fromGbId, data);
		break;
	default:
		break;
	}
}



void dmg07::handle_answer_pingphase(int fromGbId, byte dat)
{

		switch (answerbytes_buffer[fromGbId].size())
		{
		case 0:
		{
			if (dat == PING_SIGNAL)
			{
				answerbytes_buffer[fromGbId].push_back(dat);
				//last_trans_nr[fromGbId] = transfer_count;
			}

			if (dat == SYNC_SIGNAL && fromGbId == 0) //&& ((transfer_count % 4) == 0) )
			{
				answerbytes_buffer[fromGbId].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

				break;

			}
			break;
		}
		case 1:
		{
			if (dat == answerbytes_buffer[fromGbId].at(0)) //&& //last_trans_nr[fromGbId] == (transfer_count - 1))
			{
				answerbytes_buffer[fromGbId].push_back(dat);
	
				if (dat == PING_SIGNAL)
				{
					enter_status |= (byte)((byte)1 << (byte)(fromGbId + 4));
				}

			}
			else if (dat == SYNC_SIGNAL && fromGbId == 0)
			{
				answerbytes_buffer[fromGbId].clear();
				answerbytes_buffer[fromGbId].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

			}
			else answerbytes_buffer[fromGbId].clear();


			break;
		}
		case 2:
		{

			if (dat == SYNC_SIGNAL && answerbytes_buffer[fromGbId].at(0) == SYNC_SIGNAL && answerbytes_buffer[fromGbId].at(1) == SYNC_SIGNAL && fromGbId == 0) //&& //last_trans_nr[fromGbId] == (transfer_count - 1))
			{
				current_state = SYNC_PHASE;
				answerbytes_buffer[fromGbId].push_back(dat);
				break;


			}
			else if (answerbytes_buffer[fromGbId].at(0) == PING_SIGNAL && answerbytes_buffer[fromGbId].at(1) == PING_SIGNAL)
			{
				answerbytes_buffer[fromGbId].push_back(dat);
				//last_trans_nr[fromGbId] = transfer_count;
				
				if (!transfer_rate) {
					transfer_rate = dat;

					int delay_in_microseconds = 13221;
					delay_between_packages_in_clocks = microseconds_to_clocks(delay_in_microseconds);
				}
					
			}
			
			else if (dat == SYNC_SIGNAL && fromGbId == 0)//&& ((transfer_count % 4) == 0))
			{
				answerbytes_buffer[fromGbId].clear();
				answerbytes_buffer[fromGbId].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

			}
			else answerbytes_buffer[fromGbId].clear();

			break;
		}
		case 3:
		{
			if ((dat == SYNC_SIGNAL || dat == 0x00) && answerbytes_buffer[fromGbId].at(0) == SYNC_SIGNAL && answerbytes_buffer[fromGbId].at(1) == SYNC_SIGNAL && answerbytes_buffer[fromGbId].at(2) == SYNC_SIGNAL && fromGbId == 0) //&& //last_trans_nr[fromGbId] == (transfer_count - 1))
			{

				current_state = SYNC_PHASE;
				answerbytes_buffer[fromGbId].clear();


			}
			else if (answerbytes_buffer[fromGbId].at(0) == PING_SIGNAL && answerbytes_buffer[fromGbId].at(1) == PING_SIGNAL)
			{
				
					packet_size = dat;
				
			}
			else if (dat == SYNC_SIGNAL && fromGbId == 0)
			{
				answerbytes_buffer[fromGbId].clear();
				answerbytes_buffer[fromGbId].push_back(dat);
				first_aa_trans_nr = transfer_count % 4;
				current_state = SYNC_PHASE;

			}

			answerbytes_buffer[fromGbId].clear();

			break;
		}
		}

}


void dmg07::handle_answer_for_restart_pingphase(int fromGbId, byte dat) {


		answerbytes_buffer[fromGbId].push_back(dat);
		int size = answerbytes_buffer[fromGbId].size();

		if (size >= 3) {
			bool resetDetected = true;
			for (int i = 1; i <= 3; i++) {
				if (answerbytes_buffer[fromGbId][size - i] != RESET_SIGNAL) {
					resetDetected = false;
					break;
				}
			}

			if (resetDetected) {

				answerbytes_buffer[fromGbId].clear();
				bytes_to_send.clear();
				restart_in = (int)(current_packet_size) * 4;
				restart_in += transfer_count;
				received_pingphase_restart_demand = true;


				for (int i = 0; i < (current_packet_size * 4); i++)
				{
					bytes_to_send.push_back(RESET_SIGNAL);
				}
			}

		}

}


byte dmg07::send_byte(byte which, byte dat)
{
	byte ret = v_gb[which]->receive_from_linkcable(dat);
	handle_answer(which, ret);

	
	if (which == 0) log_traffic(0, dat);
	log_traffic(which + 1, ret);

	return ret; 
}

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
	
		broadcast_byte(0xCC);
		master_is_synced = transfer_count % 4 == 3;
		return;
	}

	if (++phase_byte_count >= (packet_size * 4) && transfer_count % 4 == 0)
	{
		broadcast_byte(0xCC);


		

		// The calculated timing for package 4 needs some more research. 
		// You can find my note here: https://docs.google.com/spreadsheets/d/18ZbnyVGBHlae1XmvoGyw5pjNIqwN_2P2pgniL2ToRI4/edit?gid=103971155#gid=103971155 
		// For now I use the exact meassured values from https://github.com/bbbbbr/gbdk-gb-4-player/tree/main/hardware_data_and_notes/logic_analyzer_captures

		switch (packet_size)
		{
		case 1:
		{
			switch (transfer_rate)
			{
			case 0x10: // Nekketsu Koukou Dodgeball Bu : Kyouteki!Toukyuu Senshi no Maki(Japan) & Super Momotarou Dentetsu (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(937);
				delay_between_packages_in_clocks = microseconds_to_clocks(13550);
				break;
			case 0x30: // Nekketsu Koukou Dodgeball Bu : Kyouteki!Toukyuu Senshi no Maki(Japan) & Super Momotarou Dentetsu (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(1192);
				delay_between_packages_in_clocks = microseconds_to_clocks(12919);
				break;
			case 0x31: // America Oudan Ultra Quiz (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(1192);
				delay_between_packages_in_clocks = microseconds_to_clocks(13919);
				break;
			case 0x60: //Uno: Small World 1 (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(1512);
				delay_between_packages_in_clocks = microseconds_to_clocks(11974);
				break;
			case 0xA0:  // Jinsei Game Densetsu (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(1933);
				delay_between_packages_in_clocks = microseconds_to_clocks(10713);
				break;
			case 0xA8:  // Nekketsu Downtown Koushinkyoku: Dokodemo Daiundoukai (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(1933);
				delay_between_packages_in_clocks = microseconds_to_clocks(18694);
				break;
			case 0xAF: //Trump Boy II (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(1933);
				delay_between_packages_in_clocks = microseconds_to_clocks(25678);
				break;
			case 0xF0:  // Janshiro Games & Jantaku Boy
				delay_between_bytes_in_clocks = microseconds_to_clocks(2458);
				delay_between_packages_in_clocks = microseconds_to_clocks(9137);
				break;
			case 0xF1:  // Chachamaru Panic (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(2458);
				delay_between_packages_in_clocks = microseconds_to_clocks(11135);
				break;
			case 0xF3: // Yoshi's Cookie
				delay_between_bytes_in_clocks = microseconds_to_clocks(2458);
				delay_between_packages_in_clocks = microseconds_to_clocks(12130);
				break;
			case 0xF4: //Super R.C.Pro - Am
				delay_between_bytes_in_clocks = microseconds_to_clocks(2458);
				delay_between_packages_in_clocks = microseconds_to_clocks(13127);
				break;
			case 0xFF:  //Faceball 2000 & Trax & Wave Race
				delay_between_bytes_in_clocks = microseconds_to_clocks(2458);
				delay_between_packages_in_clocks = microseconds_to_clocks(24101);
				break;
			default:

				//This should be the nearly calculated timing for package size 1 
				//needs more research
				int delay_between_byte_in_microseconds = 881 + (106 * (transfer_rate >> 4));
				int delay_between_package_in_microseconds = 13865 - (31 * (transfer_rate >> 4)) + ((transfer_rate & 0x0F) * 1000);
				delay_between_bytes_in_clocks = microseconds_to_clocks(delay_between_byte_in_microseconds);
				delay_between_packages_in_clocks = microseconds_to_clocks(delay_between_package_in_microseconds);


				break;
			}
			break;
		}
		case 3: {
			if (transfer_rate == 0x85) // Top Rank Tennis (Europe)
			{
				delay_between_bytes_in_clocks = microseconds_to_clocks(1722);
				delay_between_packages_in_clocks = microseconds_to_clocks(2105);
				break;
			}
			break;
		}
		case 4: {
			switch (transfer_rate)
			{
			case 0x00: // F1 Pole Position (F-1 Hero GB '92: The Graded Driver in Japan) & World Circuit Series (F-1 Spirit in Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(881);
				delay_between_packages_in_clocks = microseconds_to_clocks(1864);
				break;
			case 0x28: //F-1 Race
				delay_between_bytes_in_clocks = microseconds_to_clocks(1091);
				delay_between_packages_in_clocks = microseconds_to_clocks(6692);
				break;
			case 0xFF: //Nakajima Satoru - F-1 Hero GB - World Championship '91 (Japan)
				delay_between_bytes_in_clocks = microseconds_to_clocks(2457);
				delay_between_packages_in_clocks = microseconds_to_clocks(2840);
				break;

			}
			break;
		}
		default:
			break;
		}
		
		//delay_between_bytes_in_clocks = microseconds_to_clocks(1932);
		//delay_between_packages_in_clocks = microseconds_to_clocks(18610);
		

		current_packet_size = packet_size;
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


void dmg07::process_transmission_phase()
{

	// start buffering data
	if (bytes_to_send.empty())
	{
		//Get Packets
		for (byte i = 0; i < v_gb.size(); i++)
		{
			byte ret = v_gb[i]->receive_from_linkcable(0x00);
			if (i == 0) handle_answer(i, ret);


			if (i == 0)log_traffic(0, 0x00);
			log_traffic(i + 1, ret);


			if (trans_buffer[i].size() < current_packet_size)
			{
				trans_buffer[i].push_back(ret);

			}

		}
		// if ready fill queue bytes_to_send
		//if (++delay == ((packet_size * 4) ))
		if (++delay == ((current_packet_size * 4) - 1))
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
			byte ret = v_gb[i]->receive_from_linkcable(next_byte);

			if (i == 0)log_traffic(0, next_byte);
			log_traffic(i + 1, ret);

			if (trans_buffer[i].size() < current_packet_size && delay > 0)
			{
				trans_buffer[i].push_back(ret);
			}

			if (i == 0) handle_answer(i, ret);

		

		}
		delay++;

		//refill queue
		if (bytes_to_send.empty())
		{

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

	//if (transfer_count == restart_in)
	if(received_pingphase_restart_demand && transfer_count >= restart_in)
	{
		reset();
		restart_pingphase();
	
	}

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
			for (int j = 0; j < current_packet_size; j++)
			{
				bytes_to_send.push_back(0);
			}
			
		}
	}
}

void dmg07::process()
{
	
	//if (!is_ready_for_next_tik() || !all_IE_are_handled()) return;
	//if (this->v_gb[0]->get_cpu()->get_clock() < this->seri_occer) return;
	uint32_t now = this->v_gb[0]->get_cpu()->get_clock();
	if ((int32_t)(now - this->seri_occer) < 0) {

		return;
	}

	switch (current_state)
	{
		case PING_PHASE: send_each_ping_byte(); break;
		case SYNC_PHASE: send_sync_bytes(); break;
		case TRANSMISSION_PHASE: process_transmission_phase(); break;
	
	}

	transfer_count++;
	update_seri_occer();
}


/* netplay support functions*/
	

size_t dmg07::get_state_size(void)
{
	size_t ret = 0;
	serializer s(&ret, serializer::COUNT);

	size_t size;
	for (size_t i = 0; i < 4; i++)
	{
		size = trans_buffer[i].size();
		s_VAR(size);
		s_ARRAY(trans_buffer[i].data());
		size = answerbytes_buffer[i].size();
		s_VAR(size);
		s_ARRAY(answerbytes_buffer[i].data());
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

	size_t size;
	for (size_t i = 0; i < 4; i++)
	{
		size = trans_buffer[i].size();
		s_VAR(size);
		if(size) s_ARRAY(trans_buffer[i].data());
		size = answerbytes_buffer[i].size();
		s_VAR(size);
		if (size) s_ARRAY(answerbytes_buffer[i].data());
	}
	size = bytes_to_send.size();
	s_VAR(size);
	if (size) s_ARRAY(bytes_to_send.data());


	serialize(s);
}

void dmg07::restore_state_mem(void* buf)
{
	serializer s(buf, serializer::LOAD_BUF);

	size_t size;

	for (int i = 0; i < 4; i++)
	{
		// trans_buffer
		s_VAR(size);
		trans_buffer[i].resize(size);
		if (size) s_ARRAY(trans_buffer[i].data());

		// answerbytes_buffer
		s_VAR(size);
		answerbytes_buffer[i].resize(size);
		if (size) s_ARRAY(answerbytes_buffer[i].data());
	}

	// bytes_to_send
	s_VAR(size);
	bytes_to_send.resize(size);
	if (size) s_ARRAY(bytes_to_send.data());

	serialize(s);
}



void dmg07::serialize(serializer& s) 
{

	s_VAR(current_state);
	s_VAR(delay_between_bytes_in_clocks);
	s_VAR(delay_between_packages_in_clocks);
	s_VAR(last_log_clock);
	s_VAR(transfer_count);
	s_VAR(phase_byte_count);
	s_VAR(restart_in);
	s_VAR(first_aa_trans_nr);
	s_VAR(sync_trans_nr);
	s_VAR(delay);
	s_VAR(seri_occer);
	s_VAR(transfer_count);
	s_VAR(phase_byte_count);
	s_VAR(enter_status);
	s_VAR(current_packet_size);
	s_VAR(packet_size);
	s_VAR(transfer_speed);
	s_VAR(transfer_rate);
	s_VAR(ready_to_sync_master);
	s_VAR(master_is_synced);
	s_VAR(received_pingphase_restart_demand);
	s_VAR(use_v_gb_size);

	s_ARRAY(in_data_buffer);
	s_ARRAY(last_trans_nr);
	
	

}








