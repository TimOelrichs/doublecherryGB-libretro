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


#include "./include/link_master_device.hpp"

extern bool logging_allowed;
extern int emulated_gbs;

byte link_master_device::send_byte(byte which, byte dat)
{
	byte ret = v_gb[which]->get_cpu()->receive_from_linkcable(dat);

	if (which == 0) log_traffic(0, dat);
	log_traffic(which + 1, ret);

	return ret; 
}

void link_master_device::broadcast_byte(byte dat)
{
	for (byte i = 0x00; i < (byte)v_gb.size(); i++)
	{
		send_byte(i, dat);
	}
}

void link_master_device::log_traffic(byte id, byte b) {

	if (logging_allowed) {
		std::string filePath = "./4p_log.csv";
		std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
		if (id < 4)
		{
			ofs << "" << std::hex << (int)b << ",";

		}
		else
		{
			ofs << "" << std::hex << (int)b << std::endl;
		}

		ofs.close();
	}
};

bool link_master_device::is_ready_for_next_tik() {
	return (v_gb[0]->get_cpu()->get_clock() >= seri_occer) && (v_gb[0]->get_regs()->SC == 0x80); 
}

void link_master_device::update_seri_occer() {
	seri_occer = v_gb[0]->get_cpu()->get_clock() + transfer_speed;
}


size_t link_master_device::get_state_size(void)
{
	size_t ret = 0;
	serializer s(&ret, serializer::COUNT);
	serialize(s);
	return ret;
}



//these are all cheating functions cause, that's nothing real link hardware could do,
//but helpful to get a hack working 


bool link_master_device::all_IE_are_handled()
{
	int connected_gbs = use_v_gb_size ? v_gb.size() : emulated_gbs; 

	for (int i = 0; i < connected_gbs; i++)
	{
		if ((v_gb[i]->get_regs()->IF & v_gb[i]->get_regs()->IE & INT_SERIAL)) return false;
	}
	return true;

}

void link_master_device::get_all_SC_reg_data()
{
	int connected_gbs = use_v_gb_size ? v_gb.size() : emulated_gbs;

	for (int i = 0; i < connected_gbs; i++)
	{
		byte data = v_gb[i]->get_regs()->SC;
		in_data_buffer[i] = data;
	}
};

void link_master_device::get_all_SB_reg_data()
{
	int connected_gbs = use_v_gb_size ? v_gb.size() : emulated_gbs;

	for (int i = 0; i < connected_gbs; i++)
	{
		byte data = v_gb[i]->get_regs()->SB;
		in_data_buffer[i] = data;
	}
};

bool link_master_device::is_expected_data(byte data)
{
	int connected_gbs = use_v_gb_size ? v_gb.size() : emulated_gbs;

	for (int i = 0; i < connected_gbs; i++)
	{
		if (in_data_buffer[i] != data) return false;
	}
	return true;
}

bool link_master_device::gbs_are_ready_to_process() {

	get_all_SC_reg_data();
	return is_expected_data(0x80) && all_IE_are_handled();
}
