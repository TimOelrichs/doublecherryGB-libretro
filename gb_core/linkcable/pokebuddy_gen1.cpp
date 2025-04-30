/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   PKM BUDDY BOY
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


#include "./include/pokebuddy_gen1.hpp"
#include "./include/PKBuddy/inline_pkbuddy_dist_events.h"



pokebuddy_gen1::pokebuddy_gen1(std::vector<gb*> gbs) {
  
	v_gb.insert(v_gb.begin(), std::begin(gbs), std::end(gbs));

	reset();
}

byte pokebuddy_gen1::receive_from_linkcable(byte data)
{
	if (current_state == OPEN_LINK)
		return init_and_set_pkm_game_generation(data);
	
	return generation == GEN_1 ? handle_gen1(data) : handle_gen2(data);	
}

void pokebuddy_gen1::reset() {

	patch_part2 = false; 
	current_state = OPEN_LINK;
	player_selected_index = 255;
	pkbuddy_selected_index = 0;
	
	unsigned char example_block[415] = { 0x87,0x80,0x82,0x8A,0x84,0x91,0x50,0x00,0x00,0x00,0x00,0x1,0x4A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50 };
	memcpy(DATA_BLOCK.data, example_block, 415);

	std::string trainer_name = convert_string_to_name("PKBUDDY");
	memcpy(DATA_BLOCK.trainer_name, trainer_name.data(), 11);

}

void pokebuddy_gen1::handle_special_hotkey(int key) {

	if (current_state == OPEN_LINK) return;
	if (generation == GEN_1)
	{
		switch (key)
		{
			//SELECTBUTTON
		case 0x10:
		{
			pkbuddy_selected_index = (++pkbuddy_selected_index % DATA_BLOCK.species_list_size);
			std::string nick_name = convert_name_to_string(std::string((const char*)DATA_BLOCK.nicknames[pkbuddy_selected_index]));
			std::string msg_str = "PKBuddy will trade " + nick_name;
			display_message(msg_str);
			return;
		}
		//START BUTTON
		case 0x20: return;
			//START AND SELECT
		case 0x30: return;
		default:
			break;
		}

		//NUMKEYS
		if (key > DATA_BLOCK.species_list_size || key <= 0) return;

		if (key < 10)
		{
			pkbuddy_selected_index = (key - 1);
			std::string nick_name = convert_name_to_string(std::string((const char*)DATA_BLOCK.nicknames[key - 1]));
			std::string msg_str = "PKBuddy will trade " + nick_name;
			display_message(msg_str);
			return;
		}
	}
	else //gen2
	{
		switch (key)
		{
			//SELECTBUTTON
		case 0x10:
		{
			pkbuddy_selected_index = (++pkbuddy_selected_index % DATA_BLOCK_gen2.species_list_size);
			std::string nick_name = convert_name_to_string(std::string((const char*)DATA_BLOCK_gen2.nicknames[pkbuddy_selected_index]));
			std::string msg_str = "PKBuddy will trade " + nick_name;
			display_message(msg_str);
			return;
		}
		//START BUTTON
		case 0x20: return;
			//START AND SELECT
		case 0x30: return;
		default:
			break;
		}

		//NUMKEYS
		if (key > DATA_BLOCK_gen2.species_list_size || key <= 0) return;

		if (key < 10)
		{
			pkbuddy_selected_index = (key - 1);
			std::string nick_name = convert_name_to_string(std::string((const char*)DATA_BLOCK_gen2.nicknames[key - 1]));
			std::string msg_str = "PKBuddy will trade " + nick_name;
			display_message(msg_str);
			return;
		}

	}




}

byte pokebuddy_gen1::init_and_set_pkm_game_generation(byte data) {

	if (data == 0x01) {
		events_were_added = false;
		return 0x02;
	}
	if (data == 0x60)
	{
		current_state = WAIT;
		generation = GEN_1;
	}
	if (data == 0x61) {

		current_state = WAIT;
		generation = GEN_2;
	}
	return 0xfe;
}

byte pokebuddy_gen1::handle_gen1(byte data) {

	switch (current_state)
	{
	case OPEN_LINK: //unreachable, already handled
	case WAIT:
	{

		if (!events_were_added)
		{
			if (!has_owned_mew()) {

				display_message("Get your Welcome Mew!");

				pokemon mew = generate_pk_from_base_table(150, 5);
				mew.iv[0] = 0x5A;
				mew.iv[1] = 0xB5;
				insert_pokemon_into_slot(mew, 0, "Mew");
				DATA_BLOCK.species_list_size = 1;
				memcpy(DATA_BLOCK.ot_names[0], convert_string_to_name("YOSHIRA").data(), 11);

			}
			else add_event_pokemon_to_datablock();

			events_were_added = true;
		}


		if (data == 0xD0) {
			current_state = SELECT_OPTIONS;
			return 0xD4;
		}
		return 0x60;
	}

	case SELECT_OPTIONS:
	{
		if (data == 0x00) return 0xfe;
		if (data == 0xFE) current_state = TRADECENTER;
		return 0x60;

	}
	case TRADECENTER:
	{
		if (data == 0x60) current_state = TRADE_TABLE;
		return 0xD4;

	}
	case TRADE_TABLE:
	{
		if (data == 0x60) current_state = WAIT_FOR_RANDOM_BYTES;
		return 0x60;
	}
	case WAIT_FOR_RANDOM_BYTES:
	{
		if (data == 0xfd || data == 0x60 || data == 0x00) return 0xfd;

		current_state = SEND_RANDOM_BYTES;
		counter = -1;
		//FALL TROUGH
	}
	case SEND_RANDOM_BYTES:
	{
		counter++;
		if (counter < 10) return RANDOM_BYTES_BLOCK[counter];

		if (data == 0xfd) return 0xfd;
		current_state = SEND_DATA_BLOCK;
		counter = -1;

		//FALL TROUGH
	}
	case SEND_DATA_BLOCK:
	{
		counter++;
		if (counter < 415)
		{
			INCOMING_DATA_BLOCK.data[counter] = data;
			return DATA_BLOCK.data[counter];
		}
		//if(counter == 416) INCOMING_DATA_BLOCK.data[counter - 1] = data;
		if (counter < 424) return counter < 418 ? 0xFF : 0xFD;

		current_state = PATCH_DATA;
		counter = 0;
		return 0x00;
	}
	case PATCH_DATA:
	{
		switch (data)
		{
		case 0x00: return 0x00;
		case 0xFF:
		{
			if (patch_part2) {
				current_state = INIT_TRADE;
				counter = 0;
			}
			patch_part2 = true;
			return 0xFF;
		}
		default: // Any nonzero value will cause a patch
			if (!patch_part2) {
				/* Pt 1 is 0x00 - 0xFB */
				//INCOMING_DATA_BLOCK.data[data - 1] = 0xFF;
			}
			else {
				/* Pt 2 is 0xFC - 0x107
						   0xFC + in - 1
				*/
				//INCOMING_DATA_BLOCK.data[0xFB + data] = 0xFF;
			}
			return 0x00;

		}
	}
	case INIT_TRADE:
	{
		if (data == 0x00) return 0x00;
		if (data >= 0x60 && data <= 0x65 && (player_selected_index == 255)) {
			player_selected_index = data - 96;
		}

		if (counter++ < 13) return (0x60 | pkbuddy_selected_index);

		current_state = DO_TRADE;
		counter = -1;
		patch_part2 = false;


		return 0x60;


	}
	case DO_TRADE:
	{
		//cancel trade
		if (data == 0x61) {

			current_state = INIT_TRADE;
			return 0x61;
		}

		if (data == 0xfd)
		{
			current_state = WAIT_FOR_RANDOM_BYTES;

			//update DATABLOCK
			DATA_BLOCK.species[pkbuddy_selected_index] = INCOMING_DATA_BLOCK.species[player_selected_index];

			memcpy(DATA_BLOCK.pokemons[pkbuddy_selected_index].data, INCOMING_DATA_BLOCK.pokemons[player_selected_index].data, sizeof(DATA_BLOCK.pokemons[pkbuddy_selected_index].data));

			for (int i = 0; i < 11; i++)
			{
				DATA_BLOCK.ot_names[pkbuddy_selected_index][i] = INCOMING_DATA_BLOCK.ot_names[player_selected_index][i];
				DATA_BLOCK.nicknames[pkbuddy_selected_index][i] = INCOMING_DATA_BLOCK.nicknames[player_selected_index][i];
			}

			player_selected_index = 255;
			//pkbuddy_selected_index = 0x00;


			return 0xfd;
		}

		return data;
	}

	}
}

byte pokebuddy_gen1::handle_gen2(byte data) {

	//TODO implement gen2 handling
	switch (current_state)
	{
	case OPEN_LINK: //unreachable, already handled
	case WAIT:
	{

		if (!events_were_added)
		{
			generate_data_block_gen2();

			if (!has_owned_mew_gen2() || !has_owned_celebi()) 
			{
				
				if (!has_owned_celebi())
				{
					display_message("Get your Welcome Celebi!");

					pokemon_gen2 celebi = generate_pk_from_base_table_gen2(251, 5);
					celebi.move1 = 0x49;
					celebi.move2 = 0x5D;
					celebi.move3 = 0x69;
					celebi.move4 = 0xD7;
					insert_pokemon_into_slot_gen2(celebi, 0, "CELEBI");
					DATA_BLOCK_gen2.species_list_size = 1;
					memcpy(DATA_BLOCK_gen2.ot_names[0], convert_string_to_name("PKBuddy").data(), 11);

				}

				if (!has_owned_mew_gen2()) 
				{
					display_message("Get your Welcome Mew!");
					pokemon_gen2 mew = generate_pk_from_base_table_gen2(151, 5);
					insert_pokemon_into_slot_gen2(mew, 1, "Mew");
					DATA_BLOCK_gen2.species_list_size = 2;
					memcpy(DATA_BLOCK_gen2.ot_names[1], convert_string_to_name("PKBuddy").data(), 11);
				}

			}
			else add_event_pokemon_to_datablock_gen2();

			events_were_added = true;
		}


		if (data == 0xD1) {
			current_state = SELECT_OPTIONS;
			return 0xD1;
		}
		return 0x61;
	}

	case SELECT_OPTIONS:
	{
		if (data == 0xD1) return 0xD1;
		if (data == 0x00) {
			current_state = TRADECENTER;
			return 0xfe;
		}
		return 0x00;

	}
	case TRADECENTER:
	{
		if (data == 0x76) current_state = TRADE_TABLE;
		return 0x75;

	}
	case TRADE_TABLE:
	{
		if (data == 0x00) current_state = WAIT_FOR_RANDOM_BYTES;
		return 0x76;
	}
	case WAIT_FOR_RANDOM_BYTES:
	{
		if (data == 0xfd || data == 0xfe || data == 0x00) return 0xfd;

		current_state = SEND_RANDOM_BYTES;
		counter = -1;
		//FALL TROUGH
	}
	case SEND_RANDOM_BYTES:
	{
		counter++;
		if (counter < 10) return RANDOM_BYTES_BLOCK[counter];

		if (data == 0xfd) return 0xfd;
		current_state = SEND_DATA_BLOCK;
		counter = -1;

		//FALL TROUGH
	}
	case SEND_DATA_BLOCK:
	{
		counter++;
		if (counter < 441)
		{
			INCOMING_DATA_BLOCK_gen2.data[counter] = data;
			return DATA_BLOCK_gen2.data[counter];
		}
		//if(counter == 416) INCOMING_DATA_BLOCK.data[counter - 1] = data;
		//if (counter < 461) return counter < 418 ? 0xFF : 0xFD;
		if (counter < 445) return data;
		current_state = PATCH_DATA;
		counter = 0;
		return 0x00;
	}
	case PATCH_DATA:
	{
		switch (data)
		{
		case 0x00: return 0x00;
		case 0xFD: return 0xFD;
		case 0xFF:
		{
			if (patch_part2) {
				current_state = INIT_TRADE;
				counter = 0;
			}
			patch_part2 = true;
			return 0xFF;
		}
		default: // Any nonzero value will cause a patch
			if (!patch_part2) {
				/* Pt 1 is 0x00 - 0xFB */
				//INCOMING_DATA_BLOCK.data[data - 1] = 0xFF;
			}
			else {
				/* Pt 2 is 0xFC - 0x107
						   0xFC + in - 1
				*/
				//INCOMING_DATA_BLOCK.data[0xFB + data] = 0xFF;
			}
			return 0x00;

		}
	}
	case INIT_TRADE:
	{

		if (data >= 0x70 && data <= 0x75) {
			player_selected_index = data - 0x70;
			counter++;
			return (0x70 | pkbuddy_selected_index);
		}

		if (counter >= 13)
		{
			current_state = DO_TRADE;
			counter = -1;
			patch_part2 = false;
			gen2_trade_confirmed = false;
		}

		if (data == 0x00) return 0x00;
		if (data == 0x20) return 0x20;

	


	}
	case DO_TRADE:
	{

		if (data == 0x72 && !gen2_trade_confirmed) gen2_trade_confirmed = true; 
		//cancel trade
		
		if (data == 0x71 && !gen2_trade_confirmed) {

			current_state = INIT_TRADE;
			return 0x71;
		}
		

		if (data == 0xfd)
		{
			current_state = WAIT_FOR_RANDOM_BYTES;

			//update DATABLOCK
			DATA_BLOCK_gen2.species[pkbuddy_selected_index] = INCOMING_DATA_BLOCK_gen2.species[player_selected_index];

			memcpy(DATA_BLOCK_gen2.pokemons[pkbuddy_selected_index].data, INCOMING_DATA_BLOCK_gen2.pokemons[player_selected_index].data, sizeof(DATA_BLOCK_gen2.pokemons[pkbuddy_selected_index].data));

			for (int i = 0; i < 11; i++)
			{
				DATA_BLOCK_gen2.ot_names[pkbuddy_selected_index][i] = INCOMING_DATA_BLOCK_gen2.ot_names[player_selected_index][i];
				DATA_BLOCK_gen2.nicknames[pkbuddy_selected_index][i] = INCOMING_DATA_BLOCK_gen2.nicknames[player_selected_index][i];
			}

			player_selected_index = 255;
			//pkbuddy_selected_index = 0x00;


			return 0xfd;
		}

		return data;
	}

	}
}

size_t pokebuddy_gen1::get_state_size(void)
{
	size_t ret = 0;
	serializer s(&ret, serializer::COUNT);
	serialize(s);
	return ret;
}


void pokebuddy_gen1::save_state_mem(void* buf)
{
	serializer s(buf, serializer::SAVE_BUF);
	serialize(s);
}

void pokebuddy_gen1::restore_state_mem(void* buf)
{
	serializer s(buf, serializer::LOAD_BUF);
	serialize(s);
}



void pokebuddy_gen1::serialize(serializer& s)
{

	s_VAR(player_selected_index);
	s_VAR(pkbuddy_selected_index);
	s_VAR(patch_part2);
	s_VAR(current_state);
	s_VAR(counter);


	s_ARRAY(RANDOM_BYTES_BLOCK);
	s_ARRAY(DATA_BLOCK.data);
	s_ARRAY(INCOMING_DATA_BLOCK.data);
	


}