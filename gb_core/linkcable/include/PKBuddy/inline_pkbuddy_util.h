#include <cmath>
#include <string>
#include <ctime>

extern void display_message(std::string msg_str);

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


unsigned char pokebuddy_gen1::convert_ASCII2TABLE(unsigned char c, bool toUpper) {

	//A-Z && a-z
	if (c >= 0x41 && c <= 0x5A) return (unsigned char)(c + 63);
	//a-z
	if (c >= 0x61 && c <= 0x7A) return !toUpper ? (unsigned char)(c + 63) : (unsigned char)(c + 31);

	//
	switch (c)
	{
		//BLANK SPACE
	case 0x20: return 0x7f;

		//!
	case 0x21: return 0xe7;

		//.
	case 0x2E: return 0xe8;

		//?
	case 0x3F: return 0xe6;
	default:
		break;
	}

	return 0xFF;
}

std::string pokebuddy_gen1::convert_string_to_name(std::string str) {
	return convert_string_to_name(str, false);
}

std::string pokebuddy_gen1::convert_string_to_name(std::string str, bool toUpper) {


	int length = str.length() <= 10 ? str.length() : 10;
	char name[11];

	for (int i = 0; i < length; i++)
	{
		name[i] = convert_ASCII2TABLE(str.at(i), toUpper);
	}

	//set terminations
	for (int i = length; i < 11; i++)
	{
		name[i] = 0x50;
	}

	return std::string(name, 11);
}

unsigned char pokebuddy_gen1::convert_TABLE2ASCII(unsigned char c) {

	//A-Z && a-z
	if (c >= 0x41 + 63 && c <= 0x5A + 63) return (unsigned char)(c - 63);
	//a-z
	if (c >= 0x61 + 63 && c <= 0x7A + 63) return (unsigned char)(c - 63);

	//
	switch (c)
	{
		//BLANK SPACE
	case 0x7f: return 0x20;

		//!
	case 0xe7: return 0x21;

		//.
	case 0xe8: return 0x2e;

		//?
	case 0xe6: return 0x3f;
		//terminator
	case 0x50: return '\0';
	default:
		break;
	}

	return 0xFF;
}

std::string pokebuddy_gen1::convert_name_to_string(std::string str) {

	int length = str.length() <= 10 ? str.length() : 10;
	char name[11];

	for (int i = 0; i < length; i++)
	{
		name[i] = convert_TABLE2ASCII(str.at(i));
	}

	

	return std::string(name);
}


bool pokebuddy_gen1::has_owned_mew() {

	byte own = v_gb[0]->get_cpu()->read_direct(0xD309);
	return ((own >> 6) & 1);
 
}

bool pokebuddy_gen1::has_owned_mew_gen2() {
	byte own = v_gb[0]->get_cpu()->read_direct(0xDBF6);
	return ((own >> 6) & 1);
}

bool pokebuddy_gen1::has_owned_celebi() {
	byte own = v_gb[0]->get_cpu()->read_direct(0xDC03);
	return ((own >> 2) & 1);
}