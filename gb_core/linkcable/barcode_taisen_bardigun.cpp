/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Barcode Taisen Bardigun Emulation
   Copyright (C) 2024  Tim Oelrichs

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

#include "./include/barcode_taisen_bardigun.hpp"
#include "./include/barcodes/barcode_taisen_bardigun_data.h"

extern void display_message(std::string msg_str);

barcode_taisen_bardigun::barcode_taisen_bardigun() 
{
	current_barcode_index = 0;
	set_barcode();
}


byte barcode_taisen_bardigun::receive_from_linkcable(byte data)
{
	if (data_out.empty()) set_barcode();

	byte ret = data_out[0];
	data_out.erase(data_out.begin());
	return ret;
}


void barcode_taisen_bardigun::set_barcode()
{
	data_out.clear();
	switch (current_barcode_index)
	{
	case 0:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_chakanan), std::end(btb_barcode_chakanan));
		display_message("Barcode Card Chakanan::High Hearing ready to scan!");
		break;
	case 1:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_chibisshii_sword), std::end(btb_barcode_chibisshii_sword));
		display_message("Barcode Card Chibisshii::Sword ready to scan!");
		break;
	case 2:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_chibisshii_eye_beam), std::end(btb_barcode_chibisshii_eye_beam));
		display_message("Barcode Card Chibisshii::Eye Beam ready to scan!");
		break;
	case 3:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_chip_lullaby), std::end(btb_barcode_chip_lullaby));
		display_message("Barcode Card Chip::Lullaby ready to scan!");
		break;
	case 4:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_noise_ikikaeeru), std::end(btb_barcode_noise_ikikaeeru));
		display_message("Barcode Card Noise::Ikikaeeru ready to scan!");
		break;
	case 5:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_nyaari_kaenhoushaki), std::end(btb_barcode_nyaari_kaenhoushaki));
		display_message("Barcode Card Nyaari::Kaenhoushaki ready to scan!");
		break;
	case 6:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_ralph_ikikaeeru), std::end(btb_barcode_ralph_ikikaeeru));
		display_message("Barcode Card Ralph::Ikikaeeru ready to scan!");
		break;
	case 7:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_takora_fire_bolt), std::end(btb_barcode_takora_fire_bolt));
		display_message("Barcode Card Takora::Fire Bolt ready to scan!");
		break;
	case 8:
		data_out.insert(data_out.begin(), std::begin(btb_barcode_tamappi_nandemonaooru), std::end(btb_barcode_tamappi_nandemonaooru));
		display_message("Barcode Card Tamappi::Nandemonaooru ready to scan!");
		break;
	}

}

void barcode_taisen_bardigun::handle_special_hotkey(int key)
{
	if (key == 0x10) {
		current_barcode_index = ++current_barcode_index % 9;
		set_barcode();
		return;
	}

	if (key < 0 || key > 8) return; 
	if (key == current_barcode_index) return;

	current_barcode_index = key;
	set_barcode();
		
}
