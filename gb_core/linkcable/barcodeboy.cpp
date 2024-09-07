/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Barcode Boy Emulation
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


#include "./include/barcodeboy.hpp"
#include "./include/barcodes/barcode_boy_data.h"

extern void display_message(std::string msg_str);

barcodeboy::barcodeboy(std::vector<gb*> g_gb, char cart_name[18]) {
	
	memcpy(this->cart_name, cart_name, 18 * sizeof(char));
	init_barcodes(cart_name);

	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));
	this->transfer_speed = 512*14;
	v_gb[0]->set_linked_target(this);
	reset();
	
}
void barcodeboy::init_barcodes(char cart_name[18]) {

	if (!strcmp(cart_name, "BATTLE SPACE"))
	{
		barcodes.clear();
		barcodes.insert(barcodes.begin(), std::begin(battle_space_barcodes), std::end(battle_space_barcodes));
		return;
	}

	if (!strcmp(cart_name, "MONSTER MAKER"))
	{
		barcodes.clear();
		barcodes.insert(barcodes.begin(), std::begin(monster_maker_barcodes), std::end(monster_maker_barcodes));
		return;
	}

	if (!strcmp(cart_name, "KATTOBI ROAD"))
	{
		barcodes.clear();
		barcodes.insert(barcodes.begin(), std::begin(kattobi_road_barcodes), std::end(kattobi_road_barcodes));
		return;
	}

	if (!strcmp(cart_name, "FAMISTA3"))
	{
		barcodes.clear();
		barcodes.insert(barcodes.begin(), std::begin(famista3_barcodes), std::end(famista3_barcodes));
		return;
	}

	if (!strcmp(cart_name, "FAMILY JOCKEY2"))
	{
		barcodes.clear();
		barcodes.insert(barcodes.begin(), std::begin(famista3_barcodes), std::end(famista3_barcodes));
		return;
	}

}

void barcodeboy::reset() {
	barcode_index = 0;
	current_barcode = barcodes[this->barcode_index];
	in_byte_counter = 0;
	round = 0;
	is_in_mastermode = false; 
	current_barcode_index = 0;
}

void barcodeboy::process() {

	if (!is_in_mastermode) return;
	if (!is_ready_for_next_tik()) return; 
	
	update_seri_occer(); 

	if (in_byte_counter++ == 0) {
		send_byte(0, 0x02);
		return; 
	}

	if (in_byte_counter < 15) {
		byte next = this->current_barcode.at((in_byte_counter - 2));
		send_byte(0, next);
		return; 
	}

	send_byte(0, 0x03);
	in_byte_counter = 0;

	if (round++) {
		is_in_mastermode = false;
		round = 0; 
		barcode_index = (++barcode_index >= barcodes.size()) ? 0 : barcode_index;
		current_barcode = barcodes[barcode_index];
	}

}


byte barcodeboy::receive_from_linkcable(byte in_data)
{
	log_traffic(0, in_data);
	if (is_in_mastermode) return 0xFF;
	
	switch (in_byte_counter)
	{
	case 0: 
	{
		if (in_data == 0x10) in_byte_counter++;
	
		return 0xFF;
	}
	case 1:
	{
		if (in_data == 0x07) in_byte_counter++;
		else in_byte_counter = 0;

		return 0xFF;
	}
	case 2:
	{
		if (in_data == 0x10) {
			in_byte_counter++;
			return 0x10;
		}

		in_byte_counter = 0;
		return 0xFF;
	}
	case 3:
	{
		in_byte_counter = 0;

		if (in_data == 0x07) {	
			is_in_mastermode = true;
			update_seri_occer();
			return 0x07;
		}

		return 0xFF;
	}

	default:
		break;
	}

	return 0xFF;
}


void barcodeboy::handle_special_hotkey(int key) {

	if (key < 0) return;

	if (!strcmp(this->cart_name, "BATTLE SPACE"))
	{
		if (key >= battle_space_barcodes.size() && key != 0x10) return;

		//SELECT BUTTON
		if (key == 0x10) 
			current_barcode_index = ++current_barcode_index % battle_space_barcodes.size();
		else
			current_barcode_index = key;

		barcodes.clear();
		barcodes.push_back(battle_space_barcodes[current_barcode_index]);
		display_message("BARCODE BOY: Card " + battle_space_barcode_names[current_barcode_index] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "MONSTER MAKER"))
	{
		if (key >= monster_maker_barcodes.size() && key != 0x10) return;

		//SELECT BUTTON
		if (key == 0x10)
			current_barcode_index = ++current_barcode_index % monster_maker_barcodes.size();
		else
			current_barcode_index = key;

		barcodes.clear();
		barcodes.push_back(monster_maker_barcodes[current_barcode_index]);
		display_message("BARCODE BOY: Card " + monster_maker_barcode_names[current_barcode_index] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "KATTOBI ROAD"))
	{
		if (key >= kattobi_road_barcodes.size() && key != 0x10) return;

		//SELECT BUTTON
		if (key == 0x10)
			current_barcode_index = ++current_barcode_index % kattobi_road_barcodes.size();
		else
			current_barcode_index = key;

		barcodes.clear();
		barcodes.push_back(kattobi_road_barcodes[current_barcode_index]);
		display_message("BARCODE BOY: Card " + kattobi_road_barcode_names[current_barcode_index] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "FAMISTA3"))
	{
		if (key >= famista3_barcodes.size() && key != 0x10) return;

		//SELECT BUTTON
		if (key == 0x10)
			current_barcode_index = ++current_barcode_index % famista3_barcodes.size();
		else
			current_barcode_index = key;

		barcodes.clear();
		barcodes.push_back(famista3_barcodes[current_barcode_index]);
		display_message("BARCODE BOY: Card " + famista3_barcode_names[current_barcode_index] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "FAMILY JOCKEY2"))
	{
		if (key >= familiy_jockey2_barcodes.size() && key != 0x10) return;

		//SELECT BUTTON
		if (key == 0x10)
			current_barcode_index = ++current_barcode_index % familiy_jockey2_barcodes.size();
		else
			current_barcode_index = key;

		barcodes.clear();
		barcodes.push_back(familiy_jockey2_barcodes[current_barcode_index]);
		display_message("BARCODE BOY: Card " + familiy_jockey2_barcode_names[current_barcode_index] + " is ready to scan");
		return;
	}

};

void barcodeboy::serialize(serializer& s)
{

	//s_VAR(this->in_byte_counter);
	//s_VAR(this->round);
	//s_VAR(this->is_in_mastermode);

	//s_ARRAY(this->current_barcode.in_data())
	//s_ARRAY(in_data_buffer);
	//s_VAR(process_occer);


}

void barcodeboy::save_state_mem(void* buf) {};
void barcodeboy::restore_state_mem(void* buf) {};
size_t  barcodeboy::get_state_size() { return 0; };