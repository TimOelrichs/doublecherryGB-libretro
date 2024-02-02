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


pokebuddy_gen1::pokebuddy_gen1() {
  selec
}

byte pokebuddy_gen1::seri_send(byte data) {


	switch (current_state)
	{
	case INIT_LINK:
	{
		if (data == 0x01) return 0x02;
		if(data == 0x60) ++current_state; 
		return 0xfe;
		break; 
	}
	case WAIT:
	{
		if (data == 0xD0) {
			++current_state;
			return 0xD4;
		}
		return 0x60;
	}

	case SELECT_OPTIONS:
	{
		if (data == 0x00) return 0xfe;
		if (data == 0xd4) ++currentstate; 
		return 0xD4;
		break; 
	}
	case TRADECENTER:
	{
		if (data == 0xd4) ++currentstate;
		return 0xD4;
		break;
	}
	case TRADE_TABLE:
	{
		if (data == 0xfd) ++currentstate;
		return 0x60;
	}
	case WAIT_ON_TABLE:
	{
		if (data == 0xfd) ++currentstate;
		return 0xfd; 
	}
}


	void pokebuddy_gen1::generate_pk_party() {
	}