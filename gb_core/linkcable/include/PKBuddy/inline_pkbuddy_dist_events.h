#pragma once
#include "inline_pkbuddy_generator.h"
#include "inline_pkbuddy_generator_gen2.h"

void pokebuddy_gen1::add_event_pokemon_to_datablock() {

	time_t now = time(0);
	tm* ltm = localtime(&now);

	switch (ltm->tm_mon)
	{
		//january
	case 0:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Pokemon Stadium RBY Starters Week");

			int len = 3;
			int dex_no[] = { 0,3,6 };
			int levels[] = { 5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < 3; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;


		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Stadium Fighters Week");

			int len = 2;
			int dex_no[] = { 105,106 };
			int levels[] = { 20,20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Stadium EEVEE WeekK");

			int len = 1;
			int dex_no[] = { 132 };
			int levels[] = { 25 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Stadium Water WEEK");

			int len = 2;
			int dex_no[] = { 137,139 };
			int levels[] = { 20, 20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Stadium AMNESIA PSYDUCK Days");

			int len = 1;
			int dex_no[] = { 53 };
			int levels[] = { 15 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK.pokemons[0].move2 = 0x85;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;

		}
		default:
			return;
		}
		
	}
	case 1:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("FIGHT TYPE WEEK");

			int len = 5;
			int dex_no[] = { 56,61,65,66,67};
			int levels[] = { 25,40,5,28,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Valentin's Weeks");

			int len = 2;
			int dex_no[] = { 28,31 };
			int levels[] = { 5, 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK.pokemons[0].move3 = 0x85;
			DATA_BLOCK.pokemons[1].move3 = 0x85;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;

		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Legendary Birds Week");

			int len = 3;
			int dex_no[] = { 143, 144, 145 };
			int levels[] = { 50, 50, 50 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		{
			display_message("Rare Pokemon Week");

			int len = 3;
			int dex_no[] = { 142, 132, 136 };
			int levels[] = { 5, 5, 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK.pokemons[0].move2 = 0x58;
			DATA_BLOCK.pokemons[1].move3 = 0x4A;
			DATA_BLOCK.pokemons[2].move4 = 0x70;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 30:
		case 31:
		default:
			return;
		}
	}

	case 2:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Bug Type Week");

			int len = 2;
			int dex_no[] = { 122, 126};
			int levels[] = { 5, 5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK.pokemons[0].move3 = 0x31;
			DATA_BLOCK.pokemons[1].move2 = 0x58;



			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("RED Exclusives Week");

			int len = 5;
			int dex_no[] = { 22,42,55,57,124};
			int levels[] = { 5, 5, 5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("RED").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("BLUE Exclusives Week");

			int len = 5;
			int dex_no[] = { 26,36,51,68,125 };
			int levels[] = { 5, 5, 5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("BLUE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Safari Week");

			int len = 5;
			int dex_no[] = { 114,127,112,29,32 };
			int levels[] = { 30, 5, 23,31,31 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			memcpy(DATA_BLOCK.ot_names[0], convert_string_to_name("PKBuddy").data(), 11);
			set_unint16_to_bytes2(777, DATA_BLOCK.pokemons[0].originalTrainerId);

			DATA_BLOCK.pokemons[0].move3 = 0x3F;
			DATA_BLOCK.pokemons[1].move3 = 0x62;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Psychic Jynx Days");

			int len = 1;
			int dex_no[] = { 123 };
			int levels[] = { 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK.pokemons[0].move3 = 0x5E;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 3:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Team Brock Week");

			int len = 2;
			int dex_no[] = { 73, 93 };
			int levels[] = { 12, 14 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("BROCK").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Team Misty Week");

			int len = 2;
			int dex_no[] = { 119, 120 };
			int levels[] = { 18, 21 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("MISTY").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Team Lt. Surge Week");

			int len = 3;
			int dex_no[] = { 99, 24, 25 };
			int levels[] = { 21, 18, 24 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("LT.SURGE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Team Erika Week");

			int len = 3;
			int dex_no[] = { 70,113,44 };
			int levels[] = { 29,24,29 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("ERIKA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		{
			display_message("Blizzard Lapras Days");

			int len = 1;
			int dex_no[] = { 130 };
			int levels[] = { 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK.pokemons[0].move3 = 0x3B;
		

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 31:
		default:
			return;
		}
	}
	case 4:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Team Koga Week");

			int len = 4;
			int dex_no[] = { 108,108,88,109 };
			int levels[] = { 37,37,39,43};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("KOGA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Team Sabrina Week");

			int len = 4;
			int dex_no[] = { 63,111,48,64 };
			int levels[] = { 38,37,38,43};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("SABRINA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Team Blaine Week");

			int len = 4;
			int dex_no[] = { 57,76,77,58 };
			int levels[] = { 42,40,42,47 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("BLAINE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Team Giovanni Week");

			int len = 4;
			int dex_no[] = { 57,76,77,58 };
			int levels[] = { 42,40,42,47 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("GIOVANNI").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Psychic Starmie Days");

			int len = 1;
			int dex_no[] = { 120 };
			int levels[] = { 30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK.pokemons[0].move4 = 0x5E;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}
	case 5:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			
			display_message("Team Lorelei Week");

			int len = 5;
			int dex_no[] = { 86,90,79,123,130 };
			int levels[] = { 54,43,54,56,56};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("LORELEI").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Team Bruno Week");

			int len = 5;
			int dex_no[] = { 94,106,105,94,67 };
			int levels[] = { 53,55,55,56,58};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("BRUNO").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Team Agatha Week");

			int len = 5;
			int dex_no[] = { 93,41,92,23,93 };
			int levels[] = { 56,56,55,58,60 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("AGATHA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Team Lance Week");

			int len = 5;
			int dex_no[] = { 129,147,147,141,148 };
			int levels[] = { 58,56,56,60,62 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("LANCE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Fireblast Hyper Beam Ninetales Days");

			int len = 1;
			int dex_no[] = { 37 };
			int levels[] = { 40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK.pokemons[0].move1 = 0x7E;
			DATA_BLOCK.pokemons[0].move3 = 0x3F;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 6:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("University Magikarp Week");

			int len = 1;
			int dex_no[] = { 128 };
			int levels[] = { 15 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("CELADON").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			//modify default stats
			DATA_BLOCK.pokemons[0].move2 = 0x52;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Tropical Promotion to Summer Festival 2");

			int len = 3;
			int dex_no[] = { 53,71,130 };
			int levels[] = { 5,5,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK.pokemons[0].move3 = 0xA1;
			DATA_BLOCK.pokemons[1].move2 = 0x6D;
			DATA_BLOCK.pokemons[2].move4 = 0x2C;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18: 
		case 19:
		case 20:
		case 21:
		{
			display_message("ICE IN THE SUNSHINE WEEK");

			int len = 4;
			int dex_no[] = { 86,90,129,6 };
			int levels[] = { 34,25,20,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			DATA_BLOCK.pokemons[0].move4 = 0x3B;
			DATA_BLOCK.pokemons[1].move2 = 0x3B;
			DATA_BLOCK.pokemons[2].move1 = 0x3B;
			DATA_BLOCK.pokemons[2].move3 = 0x3A;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("GRASS TYPE WEEK");

			int len = 4;
			int dex_no[] = { 113,2,44,102};
			int levels[] = { 5,32,32,32};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29: 
		case 30:
		case 31:
		
		{
			display_message("Earthquake Rhydon Days");

			int len = 1;
			int dex_no[] = { 111 };
			int levels[] = { 42 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 7:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Pokemon Stamp Week");

			int len = 2;
			int dex_no[] = { 21,77};
			int levels[] = { 25,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PKMSTAMP").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			//modify default stats
			DATA_BLOCK.pokemons[0].move4 = 0x06;
			DATA_BLOCK.pokemons[1].move4 = 0x06;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("EEVEE EVOLUTION WEEK");

			int len = 3;
			int dex_no[] = { 133,134,135 };
			int levels[] = { 20,20,20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK.pokemons[0].move1 = 0x3A;
			DATA_BLOCK.pokemons[1].move1 = 0x55;
			DATA_BLOCK.pokemons[2].move1 = 0x7E;
			

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("ROCKSTAR WEEK");

			int len = 3;
			int dex_no[] = { 138,140,75 };
			int levels[] = { 40,40,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("PSYCH Week");

			int len = 5;
			int dex_no[] = { 62,63,64,95,96 };
			int levels[] = { 5,15,40,5,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			// Surfing Pikachu CoroCoro Comic Special
			// This Pokémon was available in Japan
			// from August 30 to September 30, 1997; September 13 to October 14, 1997.
			

			display_message("Surfing Pikachu CoroCoro Comic Special Event from August 30 to September 30");
			display_message("Get your SURFING PIKACHU!");

			pokemon pikachu = generate_pk_from_base_table(24, 5);
			pikachu.move3 = 0x39;

			insert_pokemon_into_slot(pikachu, 0, "Pikachu");

			memcpy(DATA_BLOCK.ot_names[0], convert_string_to_name("COROCORO").data(), 11);
			set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[0].originalTrainerId);

			return;
			
		}
			
		default:
			return;
		}
	}
	case 8:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("YELLOW MISSING WEEK");

			int len = 6;
			int dex_no[] = { 123,124,125,25,51,12 };
			int levels[] = { 30,30,30,30,5,5,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("WATER TYPE WEEK");

			int len = 5;
			int dex_no[] = { 6,117,119,130,54 };
			int levels[] = { 5,5,5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			// Surfing Pikachu CoroCoro Comic Special
			// This Pokémon was available in Japan
			// from August 30 to September 30, 1997; September 13 to October 14, 1997.
		
			display_message("Surfing Pikachu CoroCoro Comic Special Event");
			display_message("Get your SURFING PIKACHU!");

			pokemon pikachu = generate_pk_from_base_table(24, 5);
			pikachu.move3 = 0x39;
			insert_pokemon_into_slot(pikachu, 0, "Pikachu");

			memcpy(DATA_BLOCK.ot_names[0], convert_string_to_name("COROCORO").data(), 11);
			set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[0].originalTrainerId);
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("NORMAL TYPE WEEK");

			int len = 4;
			int dex_no[] = { 51,83,127,142 };
			int levels[] = { 5,5,20,20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		{

			display_message("SOLAR BEAM VICTREEBEL DAYS");
			display_message("Get your SOLAR BEAM VICTREEBEL!");

			pokemon pikachu = generate_pk_from_base_table(70, 40);
			pikachu.move4 = 0x4C;
			insert_pokemon_into_slot(pikachu, 0, "Pikachu");
			return;
		}
		case 31:
		default:
			return;
		}
	}
	case 9:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("GROUND TYPE WEEK");

			int len = 4;
			int dex_no[] = { 110,50,33,30 };
			int levels[] = { 5,26,30,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("POISON TYPE WEEK");

			int len = 4;
			int dex_no[] = { 22,108,0,68 };
			int levels[] = { 5,5,5,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			// Surfing Pikachu CoroCoro Comic Special
		// This Pokémon was available in Japan
		// from August 30 to September 30, 1997; September 13 to October 14, 1997.


			display_message("Flying Pikachu CoroCoro Comic Special Event");
			display_message("Get your FLYING PIKACHU!");

			pokemon pikachu = generate_pk_from_base_table(24, 5);
			pikachu.move3 = 0x13;

			insert_pokemon_into_slot(pikachu, 0, "Pikachu");

			memcpy(DATA_BLOCK.ot_names[0], convert_string_to_name("COROCORO").data(), 11);
			set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[0].originalTrainerId);


			return;

		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("FLYING TYPE WEEK");

			int len = 5;
			int dex_no[] = { 17,141,21,11,82 };
			int levels[] = { 36,5,2,10,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("HALLOWEEN GHOST SPECIAL");

			int len = 3;
			int dex_no[] = { 91,92,93 };
			int levels[] = { 5,25,40};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 10:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("COZY WEEK");

			int len = 4;
			int dex_no[] = { 142,78,79,35 };
			int levels[] = { 15,5,37,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("RBY Starters Evolutions WEEK");

			int len = 3;
			int dex_no[] = { 2,5,8 };
			int levels[] = { 32,36,36 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("RED Exclusives Week");

			int len = 5;
			int dex_no[] = { 22,42,55,57,124 };
			int levels[] = { 5, 5, 5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("RED").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("BLUE Exclusives Week");

			int len = 5;
			int dex_no[] = { 26,36,51,68,125 };
			int levels[] = { 5, 5, 5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("BLUE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		{
			display_message("DRAGON DAYS");

			int len = 3;
			int dex_no[] = { 146,147,149 };
			int levels[] = { 5,30,55 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);
			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 31:
		default:
			return;
		}
	}

	case 11:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Legendary Birds Week");

			int len = 3;
			int dex_no[] = { 143, 144, 145 };
			int levels[] = { 50, 50, 50 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("FIRE TYPE WEEK");

			int len = 5;
			int dex_no[] = { 125,58,3,36,76 };
			int levels[] = { 30,30,5,5,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("ELECTRIC TYPE WEEK");

			int len = 3;
			int dex_no[] = { 100,81,25 };
			int levels[] = { 30,30,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("FAIRY TYPE WEEK");

			int len = 5;
			int dex_no[] = { 121,39,35,37,77 };
			int levels[] = { 40,40,40,40,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("New YEARS MEW");
			display_message("Get your MEW!");

			pokemon pikachu = generate_pk_from_base_table(150, 5);
		
			insert_pokemon_into_slot(pikachu, 0, "MEW");

			return;
		}
		default:
			return;
		}
	}
	default:
		return;
	}

}

void pokebuddy_gen1::add_event_pokemon_to_datablock_gen2() {

	time_t now = time(0);
	tm* ltm = localtime(&now);

	switch (ltm->tm_mon)
	{
		//january
	case 0:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Grass Type Week");

			int len = 5;
			int dex_no[] = { 191,3,6 };
			int levels[] = { 5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < 3; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;


		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Stadium Fighters Week");

			int len = 2;
			int dex_no[] = { 105,106 };
			int levels[] = { 20,20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Stadium EEVEE WeekK");

			int len = 1;
			int dex_no[] = { 132 };
			int levels[] = { 25 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Stadium Water WEEK");

			int len = 2;
			int dex_no[] = { 137,139 };
			int levels[] = { 20, 20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Stadium AMNESIA PSYDUCK Days");

			int len = 1;
			int dex_no[] = { 53 };
			int levels[] = { 15 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("STADIUM").data(), 11);
				set_unint16_to_bytes2(2000, DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK_gen2.pokemons[0].move2 = 0x85;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;

		}
		default:
			return;
		}

	}
	case 1:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("FIGHT TYPE WEEK");

			int len = 5;
			int dex_no[] = { 56,61,65,66,67 };
			int levels[] = { 25,40,5,28,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Valentin's Weeks");

			int len = 2;
			int dex_no[] = { 28,31 };
			int levels[] = { 5, 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK_gen2.pokemons[0].move3 = 0x85;
			DATA_BLOCK_gen2.pokemons[1].move3 = 0x85;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;

		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Legendary Birds Week");

			int len = 3;
			int dex_no[] = { 143, 144, 145 };
			int levels[] = { 50, 50, 50 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		{
			display_message("Rare Pokemon Week");

			int len = 3;
			int dex_no[] = { 142, 132, 136 };
			int levels[] = { 5, 5, 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK_gen2.pokemons[0].move2 = 0x58;
			DATA_BLOCK_gen2.pokemons[1].move3 = 0x4A;
			DATA_BLOCK_gen2.pokemons[2].move4 = 0x70;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 30:
		case 31:
		default:
			return;
		}
	}

	case 2:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Bug Type Week");

			int len = 2;
			int dex_no[] = { 122, 126 };
			int levels[] = { 5, 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK_gen2.pokemons[0].move3 = 0x31;
			DATA_BLOCK_gen2.pokemons[1].move2 = 0x58;



			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("RED Exclusives Week");

			int len = 5;
			int dex_no[] = { 22,42,55,57,124 };
			int levels[] = { 5, 5, 5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("RED").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("BLUE Exclusives Week");

			int len = 5;
			int dex_no[] = { 26,36,51,68,125 };
			int levels[] = { 5, 5, 5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("BLUE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Safari Week");

			int len = 5;
			int dex_no[] = { 114,127,112,29,32 };
			int levels[] = { 30, 5, 23,31,31 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			memcpy(DATA_BLOCK_gen2.ot_names[0], convert_string_to_name("PKBuddy").data(), 11);
			set_unint16_to_bytes2(777, DATA_BLOCK_gen2.pokemons[0].originalTrainerId);

			DATA_BLOCK_gen2.pokemons[0].move3 = 0x3F;
			DATA_BLOCK_gen2.pokemons[1].move3 = 0x62;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Psychic Jynx Days");

			int len = 1;
			int dex_no[] = { 123 };
			int levels[] = { 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move3 = 0x5E;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 3:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Team Brock Week");

			int len = 2;
			int dex_no[] = { 73, 93 };
			int levels[] = { 12, 14 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("BROCK").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Team Misty Week");

			int len = 2;
			int dex_no[] = { 119, 120 };
			int levels[] = { 18, 21 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("MISTY").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Team Lt. Surge Week");

			int len = 3;
			int dex_no[] = { 99, 24, 25 };
			int levels[] = { 21, 18, 24 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("LT.SURGE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Team Erika Week");

			int len = 3;
			int dex_no[] = { 70,113,44 };
			int levels[] = { 29,24,29 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("ERIKA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		{
			display_message("Blizzard Lapras Days");

			int len = 1;
			int dex_no[] = { 130 };
			int levels[] = { 5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move3 = 0x3B;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 31:
		default:
			return;
		}
	}
	case 4:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Team Koga Week");

			int len = 4;
			int dex_no[] = { 108,108,88,109 };
			int levels[] = { 37,37,39,43 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("KOGA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Team Sabrina Week");

			int len = 4;
			int dex_no[] = { 63,111,48,64 };
			int levels[] = { 38,37,38,43 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("SABRINA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Team Blaine Week");

			int len = 4;
			int dex_no[] = { 57,76,77,58 };
			int levels[] = { 42,40,42,47 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("BLAINE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Team Giovanni Week");

			int len = 4;
			int dex_no[] = { 57,76,77,58 };
			int levels[] = { 42,40,42,47 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("GIOVANNI").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Psychic Starmie Days");

			int len = 1;
			int dex_no[] = { 120 };
			int levels[] = { 30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move4 = 0x5E;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}
	case 5:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{

			display_message("Team Lorelei Week");

			int len = 5;
			int dex_no[] = { 86,90,79,123,130 };
			int levels[] = { 54,43,54,56,56 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("LORELEI").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;

		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Team Bruno Week");

			int len = 5;
			int dex_no[] = { 94,106,105,94,67 };
			int levels[] = { 53,55,55,56,58 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("BRUNO").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Team Agatha Week");

			int len = 5;
			int dex_no[] = { 93,41,92,23,93 };
			int levels[] = { 56,56,55,58,60 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("AGATHA").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Team Lance Week");

			int len = 5;
			int dex_no[] = { 129,147,147,141,148 };
			int levels[] = { 58,56,56,60,62 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("LANCE").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Fireblast Hyper Beam Ninetales Days");

			int len = 1;
			int dex_no[] = { 37 };
			int levels[] = { 40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move1 = 0x7E;
			DATA_BLOCK_gen2.pokemons[0].move3 = 0x3F;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 6:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("University Magikarp Week");

			int len = 1;
			int dex_no[] = { 128 };
			int levels[] = { 15 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("CELADON").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move2 = 0x52;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Tropical Promotion to Summer Festival 2");

			int len = 3;
			int dex_no[] = { 53,71,130 };
			int levels[] = { 5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PCNYa").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			DATA_BLOCK_gen2.pokemons[0].move3 = 0xA1;
			DATA_BLOCK_gen2.pokemons[1].move2 = 0x6D;
			DATA_BLOCK_gen2.pokemons[2].move4 = 0x2C;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("ICE IN THE SUNSHINE WEEK");

			int len = 4;
			int dex_no[] = { 86,90,129,6 };
			int levels[] = { 34,25,20,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			DATA_BLOCK_gen2.pokemons[0].move4 = 0x3B;
			DATA_BLOCK_gen2.pokemons[1].move2 = 0x3B;
			DATA_BLOCK_gen2.pokemons[2].move1 = 0x3B;
			DATA_BLOCK_gen2.pokemons[2].move3 = 0x3A;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("GRASS TYPE WEEK");

			int len = 4;
			int dex_no[] = { 113,2,44,102 };
			int levels[] = { 5,32,32,32 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:

		{
			display_message("Earthquake Rhydon Days");

			int len = 1;
			int dex_no[] = { 111 };
			int levels[] = { 42 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 7:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Pokemon Stamp Week");

			int len = 2;
			int dex_no[] = { 21,77 };
			int levels[] = { 25,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				memcpy(DATA_BLOCK_gen2.ot_names[i], convert_string_to_name("PKMSTAMP").data(), 11);
				set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[i].originalTrainerId);
			}

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move4 = 0x06;
			DATA_BLOCK_gen2.pokemons[1].move4 = 0x06;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("EEVEE EVOLUTION WEEK");

			int len = 3;
			int dex_no[] = { 133,134,135 };
			int levels[] = { 20,20,20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			DATA_BLOCK_gen2.pokemons[0].move1 = 0x3A;
			DATA_BLOCK_gen2.pokemons[1].move1 = 0x55;
			DATA_BLOCK_gen2.pokemons[2].move1 = 0x7E;


			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("ROCKSTAR WEEK");

			int len = 3;
			int dex_no[] = { 138,140,75 };
			int levels[] = { 40,40,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("PSYCH Week");

			int len = 5;
			int dex_no[] = { 62,63,64,95,96 };
			int levels[] = { 5,15,40,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			// Surfing Pikachu CoroCoro Comic Special
			// This Pokémon was available in Japan
			// from August 30 to September 30, 1997; September 13 to October 14, 1997.


			display_message("Surfing Pikachu CoroCoro Comic Special Event from August 30 to September 30");
			display_message("Get your SURFING PIKACHU!");

			pokemon pikachu = generate_pk_from_base_table(24, 5);
			pikachu.move3 = 0x39;

			insert_pokemon_into_slot(pikachu, 0, "Pikachu");

			memcpy(DATA_BLOCK_gen2.ot_names[0], convert_string_to_name("COROCORO").data(), 11);
			set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[0].originalTrainerId);

			return;

		}

		default:
			return;
		}
	}
	case 8:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("YELLOW MISSING WEEK");

			int len = 6;
			int dex_no[] = { 123,124,125,25,51,12 };
			int levels[] = { 30,30,30,30,5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("WATER TYPE WEEK");

			int len = 5;
			int dex_no[] = { 6,117,119,130,54 };
			int levels[] = { 5,5,5,5,30 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			// Surfing Pikachu CoroCoro Comic Special
			// This Pokémon was available in Japan
			// from August 30 to September 30, 1997; September 13 to October 14, 1997.

			display_message("Surfing Pikachu CoroCoro Comic Special Event");
			display_message("Get your SURFING PIKACHU!");

			pokemon pikachu = generate_pk_from_base_table(24, 5);
			pikachu.move3 = 0x39;
			insert_pokemon_into_slot(pikachu, 0, "Pikachu");

			memcpy(DATA_BLOCK_gen2.ot_names[0], convert_string_to_name("COROCORO").data(), 11);
			set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[0].originalTrainerId);
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("NORMAL TYPE WEEK");

			int len = 4;
			int dex_no[] = { 51,83,127,142 };
			int levels[] = { 5,5,20,20 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		{

			display_message("SOLAR BEAM VICTREEBEL DAYS");
			display_message("Get your SOLAR BEAM VICTREEBEL!");

			pokemon pikachu = generate_pk_from_base_table(70, 40);
			pikachu.move4 = 0x4C;
			insert_pokemon_into_slot(pikachu, 0, "Pikachu");
			return;
		}
		case 31:
		default:
			return;
		}
	}
	case 9: //October
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Power Plant Pokemon Week");

			int len = 4;
			int dex_no[] = { 172,81,239,100 };
			int levels[] = { 5,5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move3 = 0x92;
			DATA_BLOCK_gen2.pokemons[1].move2 = 0x61;
			DATA_BLOCK_gen2.pokemons[2].move3 = 0x92;
			DATA_BLOCK_gen2.pokemons[3].move2 = 0x61;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Celebi WEEK");

			int len = 1;
			int dex_no[] = { 251};
			int levels[] = { 5};

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			// Surfing Pikachu CoroCoro Comic Special
		// This Pokémon was available in Japan
		// from August 30 to September 30, 1997; September 13 to October 14, 1997.


			display_message("Flying Pikachu CoroCoro Comic Special Event");
			display_message("Get your FLYING PIKACHU!");

			pokemon pikachu = generate_pk_from_base_table(24, 5);
			pikachu.move3 = 0x13;

			insert_pokemon_into_slot(pikachu, 0, "Pikachu");

			memcpy(DATA_BLOCK_gen2.ot_names[0], convert_string_to_name("COROCORO").data(), 11);
			set_unint16_to_bytes2(std::rand(), DATA_BLOCK_gen2.pokemons[0].originalTrainerId);


			return;

		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Suicune WEEK");

			int len = 1;
			int dex_no[] = { 240};
			int levels[] = { 40};

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Scary Face Pokemon Week");

			int len = 3;
			int dex_no[] = { 173,174,183,172, 194 };
			int levels[] = { 5,5,5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move4 = 0xB8; // Scary Face
			DATA_BLOCK_gen2.pokemons[1].move4 = 0xB8;
			DATA_BLOCK_gen2.pokemons[2].move3 = 0xB8;
			DATA_BLOCK_gen2.pokemons[3].move3 = 0xB8;
			DATA_BLOCK_gen2.pokemons[4].move3 = 0xB8;

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		default:
			return;
		}
	}

	case 10:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Silver Cave Week");

			int len = 5;
			int dex_no[] = { 114,77,84,200,246 };
			int levels[] = { 5,5,5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move3 = 0xEB; //Synthesis
			DATA_BLOCK_gen2.pokemons[1].move3 = 0x43; //Low Kick
			DATA_BLOCK_gen2.pokemons[2].move3 = 0x43; //Low Kick
			DATA_BLOCK_gen2.pokemons[3].move3 = 0x5F; //Hypnosis
			DATA_BLOCK_gen2.pokemons[4].move3 = 0x63; //Rage

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("Union Cave Pokémon");

			int len = 5;
			int dex_no[] = { 120,98,95,118,131 };
			int levels[] = { 5,5,5,5,5};

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move3 = 0xEF; //Twister
			DATA_BLOCK_gen2.pokemons[1].move3 = 0xE8; //Metal Claw
			DATA_BLOCK_gen2.pokemons[2].move3 = 0x9F; //Sharpen
			DATA_BLOCK_gen2.pokemons[3].move3 = 0x0E; //Swords Dance
			DATA_BLOCK_gen2.pokemons[3].move4 = 0xF8; //Future Sight

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Johto Legendary Week");

			int len = 5;
			int dex_no[] = { 243,244,245,250,249 };
			int levels[] = { 40, 40, 40,40,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				make_pkm_in_slot_shiny(i);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Celebi Present SP");

			int len = 2;
			int dex_no[] = { 251, 151 };
			int levels[] = { 5, 5};

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			make_pkm_in_slot_shiny(1);
		
			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		{
			display_message("DRAGON DAYS");

			int len = 3;
			int dex_no[] = { 146,147,149 };
			int levels[] = { 5,30,55 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 31:
		default:
			return;
		}
	}

	case 11:
	{
		switch (ltm->tm_mday)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			display_message("Psychic Type Pokémon Week");

			int len = 3;
			int dex_no[] = { 63, 96, 102, 177, 122 };
			int levels[] = { 5, 5, 5, 5, 5};

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move2 = 0xC1; //Foresight
			DATA_BLOCK_gen2.pokemons[1].move3 = 0x85; //Amnesia
			DATA_BLOCK_gen2.pokemons[2].move3 = 0xE6; //Sweet Scent
			DATA_BLOCK_gen2.pokemons[3].move3 = 0xDB; //Safeguard
			DATA_BLOCK_gen2.pokemons[4].move4 = 0xAA; //Mind Reader

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			display_message("The Johto Initial Three Pokémon Week");

			int len = 5;
			int dex_no[] = { 154,157,160,250,249 };
			int levels[] = { 40,40,40,40,40 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);

			//modify default stats
			for (int i = 0; i < len; i++)
			{
				make_pkm_in_slot_shiny(i);
			}

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		{
			display_message("Rock Tunnel Pokémon");

			int len = 6;
			int dex_no[] = { 74,41,66,95,104,115 };
			int levels[] = { 5,5,5,5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move2 = 0xE5; //Rapid Spin
			DATA_BLOCK_gen2.pokemons[1].move2 = 0xAF; //Flail
			DATA_BLOCK_gen2.pokemons[2].move3 = 0x25; //Thrash
			DATA_BLOCK_gen2.pokemons[3].move3 = 0x9F; //Sharpen
			DATA_BLOCK_gen2.pokemons[4].move4 = 0x1F; //Fury Attack
			DATA_BLOCK_gen2.pokemons[5].move4 = 0xB9; //Faint Attack
			

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		{
			display_message("Ice Type Pokémon");

			int len = 4;
			int dex_no[] = { 225,86,131,220 };
			int levels[] = { 5,5,5,5 };

			event_pokemon_msg_str = "";

			generate_pk_event_party_gen2(dex_no, levels, len);
			DATA_BLOCK_gen2.pokemons[0].move2 = 0xBF; //Rapid Spin
			DATA_BLOCK_gen2.pokemons[1].move3 = 0xAF; //Flail
			DATA_BLOCK_gen2.pokemons[2].move3 = 0xF8; //Future Sight
			DATA_BLOCK_gen2.pokemons[3].move2 = 0x12; //Whirlwind
		

			display_message("Get your " + event_pokemon_msg_str + "!");
			return;
		}
		case 29:
		case 30:
		case 31:
		{
			display_message("Pokémon that Appear at Night only Week");

			int len = 5;
			int dex_no[] = { 163,198,200,215,120 };
			int levels[] = { 5,5,5,5,5 };

			generate_pk_event_party_gen2(dex_no, levels, len);

			DATA_BLOCK_gen2.pokemons[0].move3 = 0x65; //Night Shade
			DATA_BLOCK_gen2.pokemons[1].move2 = 0xFB; //Beat up
			DATA_BLOCK_gen2.pokemons[2].move3 = 0x5F; //Hypnosis
			DATA_BLOCK_gen2.pokemons[3].move3 = 0xEC; //Whirlwind
			DATA_BLOCK_gen2.pokemons[3].move3 = 0xEF; //Twister

			return;
		}
		default:
			return;
		}
	}
	default:
		return;
	}

}



/*
void event_pokemon_stamp_august_1998_japan() {


	//Pokémon Stamp Pokémon
	//Two Pokémon, Fearow and Rapidash, with the special move Pay Day were available through the Pokémon Stamp campaign
	//that ran in various magazines published by Shogakukan.

	//These Pokémon are the Kanto Pokémon Fan Club Chairman's two favorite Pokémon that he mentions in the games,
	//and also made their appearance in The Coming of Slowpoke (Eventually) in Pokémon Adventures.

	//This Pokémon was available in Japan in August 1998.
	

	display_message("Pokemon Stamp Distribution Event August 1998 - JAPAN");
	display_message("Get your PAY DAY FEAROW and PAY DAY RAPIDASH!");

	pokemon fearow = generate_pk_from_base_table(21, 25);
	fearow.move4 = 0x06;
	insert_pokemon_into_next_slot(fearow, "Fearow");

	pokemon rapidash = generate_pk_from_base_table(77, 40);
	rapidash.move4 = 0x06;
	insert_pokemon_into_next_slot(rapidash, "Rapidash");


}



/*
void event_pokemon_summer_1998_pokemon_battle_tour_pikachu(){

	// Summer 1998 Pokémon Battle Tour Pikachu
	//This Pokémon was available in Japan
	// from July 19 to August 23, 1998.
	

	display_message("Summer 1998 Pokémon Battle Tour Pikachu from July 19 to August 23");
	display_message("Get your SURFING PIKACHU!");

	pokemon pikachu = generate_pk_from_base_table(24, 5);
	pikachu.move3 = 0x39;
	insert_pokemon_into_next_slot(pikachu, "Pikachu");

}

void event_pokemon_n64_surfing_pikachu() {

	//This Pokémon was available in Japan
	//from September 21 to October 31, 1997.
	

	display_message("Nintendo 64 Surfing Pikachu 1997 - September 21 to October 31");
	display_message("Get your SURFING PIKACHU!");

	pokemon pikachu = generate_pk_from_base_table(24, 5);
	pikachu.move3 = 0x39;
	insert_pokemon_into_next_slot(pikachu, "Pikachu");

}

void event_pokemon_university_magikarp_july_1998_japan() {

	//University Magikarp
	//This Pokémon was available in Japan on July 1998.
	
	display_message("University Magikarp July 1998");
	display_message("Get your DRAGON RAGE MAGIKARP!");

	pokemon pkm = generate_pk_from_base_table(128, 15);
	pkm.move2 = 0x52;
	insert_pokemon_into_next_slot(pikachu, "Magikarp");


}


*/