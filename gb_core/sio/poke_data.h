#include <iostream>
#include <string>


union pokemon {
	struct {
		unsigned char species;
		unsigned char currentHp[2];
		unsigned char levelPc;
		unsigned char statusAilment;
		unsigned char type1;
		unsigned char type2;
		unsigned char itemHeld;
		unsigned char move1;
		unsigned char move2;
		unsigned char move3;
		unsigned char move4;
		unsigned char originalTrainerId[2];
		unsigned char exp[3];
		unsigned char hpEv[2];
		unsigned char attackEv[2];
		unsigned char defenseEv[2];
		unsigned char speedEv[2];
		unsigned char specialEv[2];
		unsigned char iv[2];
		unsigned char move1pp;
		unsigned char move2pp;
		unsigned char move3pp;
		unsigned char move4pp;
		unsigned char level;
		unsigned char maxHp[2];
		unsigned char attack[2];
		unsigned char defense[2];
		unsigned char speed[2];
		unsigned char special[2];
	};
	unsigned char data[44];
};

union pokemon_data_block {
	struct {
		unsigned char trainer_name[11];
		unsigned char beginn_species_list; 
		unsigned char species[6];
		unsigned char end_species_list;
		pokemon pokemons[6];
		unsigned char ot_names[6][11];
		unsigned char nicknames[6][11];
	};
	unsigned char data[415];
};


