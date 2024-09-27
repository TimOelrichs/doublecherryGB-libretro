/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   4x Four Player Adapter (DMG-07) Emulation (up to 16 Players)
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

#include "./include/dmg07x4.hpp"


dmg07x4::dmg07x4(std::vector<gb*> v_gb, int players) {

	switch (players) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 5: 
	{
		
		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 3);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 3, std::begin(v_gb) + 5);
		v_dmg07.push_back(new dmg07(_gbs2));
		break;
	}
		  
	case 6:
	{
	
		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 3);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 3, std::begin(v_gb) + 6);
		v_dmg07.push_back(new dmg07(_gbs2));
		break;
	}
	
	case 7:
	{
		//emulated_dmg07s = 2;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 7);
		v_dmg07.push_back(new dmg07(_gbs2));
		break;
	}
	case 8: {
		//emulated_dmg07s = 2;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 8);
		v_dmg07.push_back(new dmg07(_gbs2));
		break;
	}
	
	case 9: {
	//	emulated_dmg07s = 3;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 3);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 3, std::begin(v_gb) + 6);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 6, std::begin(v_gb) + 9);
		v_dmg07.push_back(new dmg07(_gbs3));
		break;
	}
	case 10:{
		//emulated_dmg07s = 3;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 7);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 7, std::begin(v_gb) + 10);
		v_dmg07.push_back(new dmg07(_gbs3));
		break;
	}
	case 11: {
		//emulated_dmg07s = 3;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 8);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 8, std::begin(v_gb) + 11);
		v_dmg07.push_back(new dmg07(_gbs3));
		break;
	}
	case 12:
	{
		//emulated_dmg07s = 3;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 8);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 8, std::begin(v_gb) + 12);
		v_dmg07.push_back(new dmg07(_gbs3));
		break;
	}
	case 13:
	{
		//emulated_dmg07s = 4;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 7);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 7, std::begin(v_gb) + 10);
		v_dmg07.push_back(new dmg07(_gbs3));

		std::vector<gb*> _gbs4;
		_gbs4.insert(_gbs4.begin(), std::begin(v_gb) + 10, std::begin(v_gb) + 13);
		v_dmg07.push_back(new dmg07(_gbs4));
		break;
	}
	case 14:
	{
		//emulated_dmg07s = 4;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 8);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 8, std::begin(v_gb) + 11);
		v_dmg07.push_back(new dmg07(_gbs3));

		std::vector<gb*> _gbs4;
		_gbs4.insert(_gbs4.begin(), std::begin(v_gb) + 11, std::begin(v_gb) + 14);
		v_dmg07.push_back(new dmg07(_gbs4));
		break;
	}
	case 15:
	{
		//emulated_dmg07s = 4;

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 8);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 8, std::begin(v_gb) + 12);
		v_dmg07.push_back(new dmg07(_gbs3));

		std::vector<gb*> _gbs4;
		_gbs4.insert(_gbs4.begin(), std::begin(v_gb) + 12, std::begin(v_gb) + 15);
		v_dmg07.push_back(new dmg07(_gbs4));
		break;
	}
	case 16:
	{

		std::vector<gb*> _gbs1;
		_gbs1.insert(_gbs1.begin(), std::begin(v_gb), std::begin(v_gb) + 4);
		v_dmg07.push_back(new dmg07(_gbs1));

		std::vector<gb*> _gbs2;
		_gbs2.insert(_gbs2.begin(), std::begin(v_gb) + 4, std::begin(v_gb) + 8);
		v_dmg07.push_back(new dmg07(_gbs2));

		std::vector<gb*> _gbs3;
		_gbs3.insert(_gbs3.begin(), std::begin(v_gb) + 8, std::begin(v_gb) + 12);
		v_dmg07.push_back(new dmg07(_gbs3));

		std::vector<gb*> _gbs4;
		_gbs4.insert(_gbs4.begin(), std::begin(v_gb) + 12, std::begin(v_gb) + 16);
		v_dmg07.push_back(new dmg07(_gbs4));
		break;
	}
	
	}

	for (int i = 0; i < v_dmg07.size(); i++)
	{
		v_dmg07[i]->use_v_gb_size = true; 
	}

}

void dmg07x4::process() {
		
	for (int i = 0; i < v_dmg07.size(); i++)
	{
		v_dmg07[i]->process();
	}	
}

void dmg07x4::reset() {

	for (int i = 0; i < v_dmg07.size(); i++)
	{
		v_dmg07[i]->reset();
	}

}


void dmg07x4::serialize(serializer& s) {

	
}


void dmg07x4::save_state_mem(void* buf) {

	for (size_t i = 0; i < v_dmg07.size(); i++)
	{
		v_dmg07[i]->save_state_mem(buf);
	}
};


void dmg07x4::restore_state_mem(void* buf) {

	for (size_t i = 0; i < v_dmg07.size(); i++)
	{
		v_dmg07[i]->save_state_mem(buf);
	}
};
size_t dmg07x4::get_state_size() { return 0; };