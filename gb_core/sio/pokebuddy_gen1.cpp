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
#include "pokebuddy_gen1.hpp"
#include <math.h>


pokebuddy_gen1::pokebuddy_gen1() {
  

	unsigned char example_block[415] = { 0x87,0x80,0x82,0x8A,0x84,0x91,0x50,0x00,0x00,0x00,0x00,0x1,0x4A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x4A,0x1,0x2c,0x4a,0x00,0x14,0x08,0x1F,0x7E,0x38,0x09,0x19,0x4,0xd2,0x3,0xd,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x4a,0x1,0x2c,0x0,0x96,0x0,0x97,0x0,0x98,0x0,0x99,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x81,0x8E,0x81,0x81,0x8E,0x50,0x00,0x00,0x00,0x00,0x00,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50,0x80,0xAB,0xA2,0xA7,0xA4,0xAC,0xB8,0x50,0x50,0x50,0x50 };
	memcpy(DATA_BLOCK.data, example_block, 415);

	pokemon mew = generate_pk_from_base_table(150, 5);
	insert_pokemon_into_datablock(mew, 0);
}

byte pokebuddy_gen1::seri_send(byte data)
{
	switch (current_state)
	{
	case OPEN_LINK:
	{
		if (data == 0x01) return 0x02;
		if (data == 0x60) current_state = WAIT;
		return 0xfe;

	}
	case WAIT:
	{
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
				INCOMING_DATA_BLOCK.data[data - 1] = 0xFE;
			}
			else {
				/* Pt 2 is 0xFC - 0x107
						   0xFC + in - 1
				*/
				INCOMING_DATA_BLOCK.data[0xFB + data] = 0xFE;
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
		if (counter++ < 13) return 0x60;

		current_state = DO_TRADE;
		counter = -1;
		patch_part2 = false;


		DATA_BLOCK.species[0] = INCOMING_DATA_BLOCK.species[player_selected_index];
	
		memcpy(DATA_BLOCK.pokemons[0].data, INCOMING_DATA_BLOCK.pokemons[player_selected_index].data, sizeof(DATA_BLOCK.pokemons[0].data));
		
		for (int i = 0; i < 11; i++)
		{
			DATA_BLOCK.ot_names[0][i] = INCOMING_DATA_BLOCK.ot_names[player_selected_index][i];
			DATA_BLOCK.nicknames[0][i] = INCOMING_DATA_BLOCK.nicknames[player_selected_index][i];
		}

		player_selected_index = 255;
		
		return 0x60;


	}
	case DO_TRADE:
	{
		if (data == 0xfd)
		{
			current_state = WAIT_FOR_RANDOM_BYTES;
			return 0xfd;
		}

		return data;
	}
	/*
	case WAIT_FOR_NEXT_DATA_BLOCK:
	{
		if (data != 0xfd) current_state = WAIT_FOR_RANDOM_BYTES;
		return 0xfd;
	}
	*/

	}
}

void pokebuddy_gen1::reset() {
	patch_part2 = false; 
	current_state = OPEN_LINK;
}

void pokebuddy_gen1::generate_pk_party() {
}

pokemon pokebuddy_gen1::generate_pk_from_base_table(int index_id, unsigned char level)
{
	pokemon_base_stats pkmn_base = pokemon_table[index_id];

	pokemon generated_pkmn;

	generated_pkmn.level = level; 
	generated_pkmn.levelPc = level; 
	generated_pkmn.species = pkmn_base.index;
	generated_pkmn.type1 = pkmn_base.type[0];
	generated_pkmn.type2 = pkmn_base.type[2];
	generated_pkmn.statusAilment = 0x00;
	generated_pkmn.itemHeld = 0x00;

	//shorten by changing pokemon union
	generated_pkmn.move1 = pkmn_base.move[0];
	generated_pkmn.move2 = pkmn_base.move[1];
	generated_pkmn.move3 = pkmn_base.move[2];
	generated_pkmn.move4 = pkmn_base.move[3];

	generated_pkmn.move1pp = 0x00;
	generated_pkmn.move2pp = 0x00;
	generated_pkmn.move3pp = 0x00;
	generated_pkmn.move4pp = 0x00;

	//TODO randomize IVs
	generated_pkmn.iv[0] = 0xFF;
	generated_pkmn.iv[1] = 0xFF;

	set_unint16_to_bytes2(0, generated_pkmn.attackEv);
	set_unint16_to_bytes2(0, generated_pkmn.defenseEv);
	set_unint16_to_bytes2(0, generated_pkmn.specialEv);
	set_unint16_to_bytes2(0, generated_pkmn.speedEv);
	set_unint16_to_bytes2(0, generated_pkmn.hpEv);


	int exp = pokemon_calculate_exp(pkmn_base, level);
	set_unint32_to_bytes3(exp, generated_pkmn.exp);

	set_unint16_to_bytes2(777, generated_pkmn.originalTrainerId);

	//calc stats

	int hp = std::floor((((2 * (pkmn_base.base_hp + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.hpEv)) / 4)) * level) / 100) + level + 10;
	set_unint16_to_bytes2(hp, generated_pkmn.maxHp);
	set_unint16_to_bytes2(hp, generated_pkmn.currentHp);

	int attack = std::floor((((2 * (pkmn_base.base_atk + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.attackEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(attack, generated_pkmn.attack);

	int defense = std::floor((((2 * (pkmn_base.base_def + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.defenseEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(defense, generated_pkmn.defense);

	int speed = std::floor((((2 * (pkmn_base.base_spd + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.speedEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(speed, generated_pkmn.speed);
	
	int special = std::floor((((2 * (pkmn_base.base_spc + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.specialEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(special, generated_pkmn.special);


	return generated_pkmn;
}


int pokebuddy_gen1::pokemon_calculate_exp(pokemon_base_stats pkm_base, int level) {

	int exp = 0;

	/* Calculate exp */
	switch (pkm_base.growth) {
	case GROWTH_FAST:
		// https://bulbapedia.bulbagarden.net/wiki/Experience#Fast
		exp = (4 * level * level * level) / 5;
		break;
	case GROWTH_MEDIUM_FAST:
		// https://bulbapedia.bulbagarden.net/wiki/Experience#Medium_Fast
		exp = (level * level * level);
		break;
	case GROWTH_MEDIUM_SLOW:
		// https://bulbapedia.bulbagarden.net/wiki/Experience#Medium_Slow
		exp = (((level * level * level) * 6 / 5) - (15 * level * level) + (100 * level) - 140);
		break;
	case GROWTH_SLOW:
		// https://bulbapedia.bulbagarden.net/wiki/Experience#Slow
		exp = (5 * level * level * level) / 4;
		break;
	default:
		
		break;
	}

	return exp; 

}

void pokebuddy_gen1::pokemon_calc_and_set_stats(pokemon pkm) {

	int hp = 0;


	
}

void pokebuddy_gen1::insert_pokemon_into_datablock(pokemon pkm, int slot) {

	//if (slot < 0 || slot > 5) return;

	DATA_BLOCK.species[slot] = pkm.species;
	memcpy(DATA_BLOCK.pokemons[slot].data, pkm.data, sizeof(pkm.data));
}



void pokebuddy_gen1::set_unint32_to_bytes3(int input, unsigned char* output_array)
{
	output_array[2] = (unsigned char)((input) & 0xFF);
	output_array[1] = (unsigned char)(((input) >> 8) & 0xFF);
	output_array[0] = (unsigned char)(((input) >> 16) & 0xFF);
}

void pokebuddy_gen1::set_unint16_to_bytes2(int input, unsigned char* output_array)
{
	output_array[1] = (unsigned char)((input) & 0xFF);
	output_array[0] = (unsigned char)(((input) >> 8) & 0xFF);
}

unsigned int pokebuddy_gen1::bytes3_to_uint32(unsigned char* output_array)
{
	unsigned int ret = 0;
	return ret | output_array[2] | output_array[1] | output_array[0];

}

unsigned int pokebuddy_gen1::bytes2_to_uint16(unsigned char* output_array)
{
	unsigned int ret = 0;
	return ret | output_array[2] | output_array[1] | output_array[0];

}


