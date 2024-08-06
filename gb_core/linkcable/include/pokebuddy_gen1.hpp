/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator - 4Players (based on TGBDual)
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

#pragma once
#include "../../gb.h"
#include "../../serializer.h"
#include "PKBuddy/poke_data.h"

#pragma once
#include "../../../libretro/dcgb_hotkey_target.hpp"


extern bool logging_allowed;
extern char cart_name[18];

enum pkm_generation {
	GEN_1,
	GEN_2
};


class pokebuddy_gen1 : public I_linkcable_target, public I_dcgb_hotkey_target, public I_savestate {

public:
	pokebuddy_gen1(std::vector<gb*> gbs);

	byte receive_from_linkcable(byte data) override;
	void reset();

	void handle_special_hotkey(int key) override;

	enum ingame_state
	{
		OPEN_LINK,
		WAIT,
		SELECT_OPTIONS,
		TRADECENTER,
		TRADE_TABLE,
		WAIT_FOR_RANDOM_BYTES,
		SEND_RANDOM_BYTES,
		SEND_DATA_BLOCK,
		PATCH_DATA,
		INIT_TRADE,
		DO_TRADE,
	};

	//I_savestate
	void save_state_mem(void* buf) override;
	void restore_state_mem(void* buf) override;
	size_t get_state_size() override;
	void serialize(serializer& s) override;

private:

	byte init_and_set_pkm_game_generation(byte data);
	byte handle_gen1(byte data);
	byte handle_gen2(byte data);

	void insert_pokemon_into_slot(pokemon pkm, unsigned char slot, std::string nickname);
	void insert_pokemon_into_next_slot(pokemon pkm, std::string nickname);
	int pokemon_calculate_exp(pokemon_base_stats pokemon, int level);
	void pokemon_calc_and_set_stats(pokemon pkm);
	void generate_pk_event_party(int *dex_no, int* levels, int size);
	pokemon generate_pk_from_base_table(int index_id, unsigned char level); 


	//utils
	unsigned char convert_ASCII2TABLE(unsigned char c, bool toUpper);
	std::string convert_string_to_name(std::string str);
	std::string convert_string_to_name(std::string str, bool toUpper);
	unsigned char convert_TABLE2ASCII(unsigned char c);
	std::string convert_name_to_string(std::string str);
	void set_unint32_to_bytes3(int input, unsigned char* output_array);
	void set_unint16_to_bytes2(int input, unsigned char* output_array);
	unsigned int bytes3_to_uint32(unsigned char* output_array);
	unsigned int bytes2_to_uint16(unsigned char* output_array);

	bool has_owned_mew();

	void add_event_pokemon_to_datablock();


	bool events_were_added = false; 
	unsigned char player_selected_index = 255;
	unsigned char pkbuddy_selected_index = 0;
	ingame_state current_state = OPEN_LINK;;

	int counter = -1;
	bool patch_part2 = false; 

	unsigned char RANDOM_BYTES_BLOCK[10] = { 0xC2, 0xF5, 0x2A, 0x61, 0x9a, 0xd4, 0x10, 0x11, 0x12, 0x13 };
	trading_data_block DATA_BLOCK, INCOMING_DATA_BLOCK;


	std::vector<gb*> v_gb;
	std::string event_pokemon_msg_str; 
	pkm_generation generation = GEN_1;

};

