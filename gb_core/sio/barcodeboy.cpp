#include "barcodeboy.hpp"

extern void display_message(std::string msg_str);

//converted to ugly C98 style for ios compatability :-|
std::string battle_space_barcodes_array[10] = {
	"4907981000301", //Berserker
	"4908052808369", //Valkyrie
	"4916911302309", //Grizzly Bear
	"4902776809367", //Magic Soldier
	"4905672306367", //Knight		
	"4912713004366", //Wraith
	"4913508504399", //Shaman		
	"4918156001351", //Thier
	"4911826551347", //Sorcerer		
	"4909062206350" //Warrior
};

std::vector<std::string> battle_space_barcodes(battle_space_barcodes_array, battle_space_barcodes_array + 10);

std::string battle_space_barcode_names_array[10] = {
	"Berserker",
	"Valkyrie",
	"Grizzly Bear",
	"Magic Soldier",
	"Knight",		
	"Wraith",
	"Shaman",	
	"Thier",
	"Sorcerer",	
	"Warrior"
};

std::vector<std::string> battle_space_barcode_names(battle_space_barcode_names_array, battle_space_barcode_names_array + 10);

std::string monster_maker_barcodes_array[6] ={
	"9998017308336", //Archer Lorian	
	"9447410810323", //Archer Elysice
	"9052091324955", //Knight Lauren
	"9322158686716", //Dragon Knight Haagun
	"9752412234900", //Warrior Diane	
	"9362462085911" //Warrior Tamron
};

std::vector<std::string> monster_maker_barcodes(monster_maker_barcodes_array, monster_maker_barcodes_array + 6);

std::string monster_maker_barcode_names_array[6] =
{
	"Archer Lorian",	
	"Archer Elysice",
	"Knight Lauren",
	"Dragon Knight Haagun",
	"Warrior Diane",
	"Warrior Tamron"
};

std::vector<std::string> monster_maker_barcode_names(monster_maker_barcode_names_array, monster_maker_barcode_names_array + 6);


std::string kattobi_road_barcodes_array[6] =
{
	"4902105002063", //Truck			
	"4901121110004", //Sedan			
	"4903301160625", //Racecar		
	"4902888119101", //Japanese Street Car
	"4901780161157", //4x4 Jeep
	"4987084410924" //F1-style racecar
};

std::vector<std::string> kattobi_road_barcodes(kattobi_road_barcodes_array, kattobi_road_barcodes_array + 6);

std::string kattobi_road_barcode_names_array[6] =
{
	"Truck",	
	"Sedan",		
	"Racecar",	
	"Japanese Street Car",
	"4x4 Jeep",
	"F1-style racecar",
};

std::vector<std::string> kattobi_road_barcode_names(kattobi_road_barcode_names_array, kattobi_road_barcode_names_array + 6);


std::string famista3_barcodes_array[4] =
{
	"8357933639923", //Home-Run Batter			
	"7814374127798", //Senior Batter			
	"9880692151263", //Swift Batter		
	"1414213562177" //Pitcher
};

std::vector<std::string> famista3_barcodes(famista3_barcodes_array, famista3_barcodes_array + 4);

std::string famista3_barcode_names_array[4] =
{
	"Home-Run Batter",			
	"Senior Batter",		
	"Swift Batter",		
	"Pitcher"
};

std::vector<std::string> famista3_barcode_names(famista3_barcode_names_array, famista3_barcode_names_array + 4);

std::string familiy_jockey2_barcodes_array[8] =
{
	"5893713522816", //A1
	"2378649896765", //A2			
	"9845554422318", //A4	
	"1509843019075", //B1
	"4232978865152", //B2
	"3572821107673", //B4
	"7164625542390", //C3
	"6319537443513" //C5
};

std::vector<std::string> familiy_jockey2_barcodes(familiy_jockey2_barcodes_array, familiy_jockey2_barcodes_array + 8);

std::string familiy_jockey2_barcode_names_array[8] =
{
	"A1",
	"A2",	
	"A4",
	"B1",
	"B2",
	"B4",
	"C3",
	"C5"
};

std::vector<std::string> familiy_jockey2_barcode_names(familiy_jockey2_barcode_names_array, familiy_jockey2_barcode_names_array + 8);


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
	seri_send_counter = 0;
	round = 0;
	is_in_mastermode = false; 
}

void barcodeboy::process() {

	if (!is_in_mastermode) return;
	if (!is_ready_for_next_tik()) return; 
	
	update_seri_occer(); 

	if (seri_send_counter++ == 0) {
		send_byte(0, 0x02);
		return; 
	}

	if (seri_send_counter < 15) {
		byte next = this->current_barcode.at((seri_send_counter - 2));
		send_byte(0, next);
		return; 
	}

	send_byte(0, 0x03);
	seri_send_counter = 0;

	if (round++) {
		is_in_mastermode = false;
		round = 0; 
		barcode_index = (++barcode_index >= barcodes.size()) ? 0 : barcode_index;
		current_barcode = barcodes[barcode_index];
	}

}


byte barcodeboy::seri_send(byte data) 
{
	log_traffic(0, data);
	if (is_in_mastermode) return 0xFF;
	
	switch (seri_send_counter)
	{
	case 0: 
	{
		if (data == 0x10) seri_send_counter++; 
	
		return 0xFF;
	}
	case 1:
	{
		if (data == 0x07) seri_send_counter++;
		else seri_send_counter = 0;

		return 0xFF;
	}
	case 2:
	{
		if (data == 0x10) {
			seri_send_counter++;
			return 0x10;
		}

		seri_send_counter = 0;
		return 0xFF;
	}
	case 3:
	{
		seri_send_counter = 0;

		if (data == 0x07) {	
			is_in_mastermode = true;
			update_seri_occer();
			return 0x07;
		}

		return 0xFF;
	}

	default:
		break;
	}

}

byte barcodeboy::get_SB_value() { return 0xFF; };
byte barcodeboy::get_SC_value() { return is_in_mastermode ? 0x81 : 0x80; };

void barcodeboy::handle_special_hotkey(int key) {

	if (!strcmp(this->cart_name, "BATTLE SPACE"))
	{
		barcodes.clear();
		if (key >= battle_space_barcodes.size()) return;
		barcodes.push_back(battle_space_barcodes[key]);
		display_message("BARCODE BOY: Card " + battle_space_barcode_names[key] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "MONSTER MAKER"))
	{
		barcodes.clear();
		if (key >= monster_maker_barcodes.size()) return;
		barcodes.push_back(monster_maker_barcodes[key]);
		display_message("BARCODE BOY: Card " + monster_maker_barcode_names[key] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "KATTOBI ROAD"))
	{
		barcodes.clear();
		if (key >= kattobi_road_barcodes.size()) return;
		barcodes.push_back(kattobi_road_barcodes[key]);
		display_message("BARCODE BOY: Card " + kattobi_road_barcode_names[key] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "FAMISTA3"))
	{
		barcodes.clear();
		if (key >= famista3_barcodes.size()) return;
		barcodes.push_back(famista3_barcodes[key]);
		display_message("BARCODE BOY: Card " + famista3_barcode_names[key] + " is ready to scan");
		return;
	}
	if (!strcmp(this->cart_name, "FAMILY JOCKEY2"))
	{
		barcodes.clear();
		if (key >= familiy_jockey2_barcodes.size()) return;
		barcodes.push_back(familiy_jockey2_barcodes[key]);
		display_message("BARCODE BOY: Card " + familiy_jockey2_barcode_names[key] + " is ready to scan");
		return;
	}

};

void barcodeboy::serialize(serializer& s)
{

	//s_VAR(this->seri_send_counter);
	//s_VAR(this->round);
	//s_VAR(this->is_in_mastermode);

	//s_ARRAY(this->current_barcode.data())
	//s_ARRAY(in_data_buffer);
	//s_VAR(process_occer);


}

void barcodeboy::save_state_mem(void* buf) {};
void barcodeboy::restore_state_mem(void* buf) {};
size_t  barcodeboy::get_state_size() { return 0; };