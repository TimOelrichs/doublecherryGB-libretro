/*--------------------------------------------------
	
   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Faceball 2000 16 Player Cable Emulation, i call it "Ring-Link"-Cable
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


#include "./include/faceball2000_cable.hpp"

extern int emulated_gbs; 

faceball2000_cable::faceball2000_cable(std::vector<gb*> g_gb) {
	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));
}


byte faceball2000_cable::receive_from_linkcable(byte data) {

	byte ret = data; 
	for (size_t i = 1; i < emulated_gbs; i++)
	{
		ret = v_gb[i]->receive_from_linkcable(ret);
	}
	return ret; 
}

