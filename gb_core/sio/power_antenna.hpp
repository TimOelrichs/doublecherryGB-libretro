/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
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


#include "../gb.h"

extern bool power_antenna_on;

class power_antenna : public I_link_target {

public:
	power_antenna() {};
	byte seri_send(byte data) override;
	byte get_SB_value() override { return 0xFF; };
	byte get_SC_value() override { return 0x80; };

	bool power_antenna_last_state = false; 
	bool power_antenna_on = false; 
	
	
};
