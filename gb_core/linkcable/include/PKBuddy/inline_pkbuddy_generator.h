#pragma once
#include "inline_pkbuddy_util.h"
#include "stdlib.h" 

void pokebuddy_gen1::generate_pk_event_party(int* dex_no, int* levels, int len) {

	DATA_BLOCK.species_list_size = 0x00;

	for (int i = 0; i < len; i++)
	{
		pokemon pkm = generate_pk_from_base_table(dex_no[i], levels[i]);
		insert_pokemon_into_next_slot(pkm, pokemon_table[dex_no[i]].name);

		//format event msg_string
		if (len == 1) event_pokemon_msg_str = std::string(pokemon_table[dex_no[i]].name);
		else {
			if ((i < (len - 1))) {
				event_pokemon_msg_str += std::string(pokemon_table[dex_no[i]].name) + ", ";
			}
			else event_pokemon_msg_str += " and " + std::string(pokemon_table[dex_no[i]].name);
		}
	}


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

	set_unint16_to_bytes2(777, generated_pkmn.originalTrainerId);

	//TODO shorten by changing pokemon union
	generated_pkmn.move1 = pkmn_base.move[0];
	generated_pkmn.move2 = pkmn_base.move[1];
	generated_pkmn.move3 = pkmn_base.move[2];
	generated_pkmn.move4 = pkmn_base.move[3];

	generated_pkmn.move1pp = 0x00;
	generated_pkmn.move2pp = 0x00;
	generated_pkmn.move3pp = 0x00;
	generated_pkmn.move4pp = 0x00;

	//randomized IVs
	set_unint16_to_bytes2(std::rand(), generated_pkmn.iv);

	//set EVs to 0
	set_unint16_to_bytes2(0, generated_pkmn.attackEv);
	set_unint16_to_bytes2(0, generated_pkmn.defenseEv);
	set_unint16_to_bytes2(0, generated_pkmn.specialEv);
	set_unint16_to_bytes2(0, generated_pkmn.speedEv);
	set_unint16_to_bytes2(0, generated_pkmn.hpEv);

	//calc and set exp
	int exp = pokemon_calculate_exp(pkmn_base, level);
	set_unint32_to_bytes3(exp, generated_pkmn.exp);

	

	//calc and set stats

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


void pokebuddy_gen1::insert_pokemon_into_slot(pokemon pkm, unsigned char slot, std::string nickname) {

	if (slot < 0 || slot > 5) return;

	DATA_BLOCK.species[slot] = pkm.species;
	memcpy(DATA_BLOCK.pokemons[slot].data, pkm.data, sizeof(pkm.data));

	std::string ot_name = convert_string_to_name("PCNYa", true);
	memcpy(DATA_BLOCK.ot_names[slot], ot_name.data(), 11);
	std::string nick_name = convert_string_to_name(nickname, true);
	memcpy(DATA_BLOCK.nicknames[slot], nick_name.data(), 11);
	
}

void pokebuddy_gen1::insert_pokemon_into_next_slot(pokemon pkm, std::string nickname) {

	unsigned char slot = (unsigned char)(DATA_BLOCK.species_list_size);

	if (slot >= 5) return;

	insert_pokemon_into_slot(pkm, slot, nickname);
	DATA_BLOCK.species_list_size++;
}



void pokebuddy_gen1::generate_data_block_gen2() {

	for (int i = 0; i < 441; i++)
	{
		DATA_BLOCK_gen2.data[i] = 0xFF;
	}

	std::string trainer_name = convert_string_to_name("PCNYa", false);
	memcpy(DATA_BLOCK_gen2.trainer_name, trainer_name.data(), 11);
	DATA_BLOCK_gen2.end_species_list[0] = 0xff;
	DATA_BLOCK_gen2.end_species_list[1] = 0xf0; //??
	DATA_BLOCK_gen2.end_species_list[2] = 0x66; //??
	DATA_BLOCK_gen2.species_list_size = 0x00;



}

void pokebuddy_gen1::generate_pk_event_party_gen2(int* dex_no, int* levels, int len) {

	DATA_BLOCK_gen2.species_list_size = 0x00;

	for (int i = 0; i < len; i++)
	{
		pokemon_gen2 pkm = generate_pk_from_base_table_gen2(dex_no[i], levels[i]);
		insert_pokemon_into_next_slot_gen2(pkm, pokemon_table[dex_no[i]-1].name);

		//format event msg_string
		if (len == 1) event_pokemon_msg_str = std::string(pokemon_table[dex_no[i]-1].name);
		else {
			if ((i < (len - 1))) {
				event_pokemon_msg_str += std::string(pokemon_table[dex_no[i]-1].name) + ", ";
			}
			else event_pokemon_msg_str += " and " + std::string(pokemon_table[dex_no[i]-1].name);
		}
	}


}

pokemon_gen2 pokebuddy_gen1::generate_pk_from_base_table_gen2(int index_id, unsigned char level)
{
	pokemon_base_stats pkmn_base = pokemon_table[index_id-1];

	pokemon_gen2 generated_pkmn;

	generated_pkmn.level = level;
	generated_pkmn.species = index_id;
	generated_pkmn.statusAilment = 0x00;
	generated_pkmn.itemHeld = 0x00;

	set_unint16_to_bytes2(777, generated_pkmn.originalTrainerId);

	//TODO shorten by changing pokemon union
	generated_pkmn.move1 = pkmn_base.move[0];
	generated_pkmn.move2 = pkmn_base.move[1];
	generated_pkmn.move3 = pkmn_base.move[2];
	generated_pkmn.move4 = pkmn_base.move[3];

	generated_pkmn.move1pp = 0x00;
	generated_pkmn.move2pp = 0x00;
	generated_pkmn.move3pp = 0x00;
	generated_pkmn.move4pp = 0x00;

	//randomized IVs
	set_unint16_to_bytes2(std::rand(), generated_pkmn.iv);

	//set EVs to 0
	set_unint16_to_bytes2(0, generated_pkmn.attackEv);
	set_unint16_to_bytes2(0, generated_pkmn.defenseEv);
	set_unint16_to_bytes2(0, generated_pkmn.specialEv);
	set_unint16_to_bytes2(0, generated_pkmn.speedEv);
	set_unint16_to_bytes2(0, generated_pkmn.hpEv);

	//calc and set exp
	int exp = pokemon_calculate_exp(pkmn_base, level);
	set_unint32_to_bytes3(exp, generated_pkmn.exp);


	//calc and set stats

	int hp = std::floor((((2 * (pkmn_base.base_hp + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.hpEv)) / 4)) * level) / 100) + level + 10;
	set_unint16_to_bytes2(hp, generated_pkmn.maxHp);
	set_unint16_to_bytes2(hp, generated_pkmn.currentHp);

	int attack = std::floor((((2 * (pkmn_base.base_atk + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.attackEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(attack, generated_pkmn.attack);

	int defense = std::floor((((2 * (pkmn_base.base_def + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.defenseEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(defense, generated_pkmn.defense);

	int speed = std::floor((((2 * (pkmn_base.base_spd + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.speedEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(speed, generated_pkmn.speed);

	int special_attack = std::floor((((2 * (pkmn_base.base_spc_atk + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.specialEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(special_attack, generated_pkmn.specialAttack);

	int special_defense = std::floor((((2 * (pkmn_base.base_spc_def + 0xF)) + std::floor(std::sqrt(bytes2_to_uint16(generated_pkmn.specialEv)) / 4)) * level) / 100) + 5;
	set_unint16_to_bytes2(special_defense, generated_pkmn.specialDefense);

	return generated_pkmn;
}


void pokebuddy_gen1::insert_pokemon_into_slot_gen2(pokemon_gen2 pkm, unsigned char slot, std::string nickname) {

	if (slot < 0 || slot > 5) return;

	DATA_BLOCK_gen2.species[slot] = pkm.species;
	memcpy(DATA_BLOCK_gen2.pokemons[slot].data, pkm.data, sizeof(pkm.data));

	memcpy(DATA_BLOCK_gen2.ot_names[slot], DATA_BLOCK_gen2.trainer_name, 11);
	std::string nick_name = convert_string_to_name(nickname, true);
	memcpy(DATA_BLOCK_gen2.nicknames[slot], nick_name.data(), 11);

}

void pokebuddy_gen1::insert_pokemon_into_next_slot_gen2(pokemon_gen2 pkm, std::string nickname) {

	unsigned char slot = (unsigned char)(DATA_BLOCK_gen2.species_list_size);

	if (slot >= 5) return;

	insert_pokemon_into_slot_gen2(pkm, slot, nickname);
	DATA_BLOCK_gen2.species_list_size++;
}

void pokebuddy_gen1::make_pkm_in_slot_shiny(int slot) {
	if (slot < 0 || slot > 5) return;

	byte shiny_attack_values[8] = { 2,3,6,7,10,11,14,15 };
	int rnd_index = std::rand() % 8;

	DATA_BLOCK_gen2.pokemons[slot].iv[0] = 0x0A | shiny_attack_values[rnd_index] << 4;
	DATA_BLOCK_gen2.pokemons[slot].iv[1] = 0xAA;

}