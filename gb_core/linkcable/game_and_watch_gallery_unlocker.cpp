/*--------------------------------------------------
   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Game and Watch Gallery Unlocker
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

/*
* Status: WIP - not working yet
* The Game and Watch Gallery 1-3 Games have a secret unlock feature over the Link Cable.
* You need to collect 80 Stars in G&W Gallery 1, and than you can link your GBC with G&W2, or G&W3 
* and press START on the Museum Note Screen. 
* 
* I tried to record data send over the linkcable. Because i could find a legit savefile,
* i used cheats to get the 80 Stars in G&W1. But i couldn't recreate it.
* Not sure if G&W1 really unlocks this feature, when using cheats.
* G&W2 send 0xFC and G&W1 answers with 0xEE. I tried to bruteforce the answer, but G&W2 doesn't react
* to any other byte than 0xEE. Maybe the games switch roles (master/slave) after 0xEE.
* Feature needs more research!
*/

#include "./include/game_and_watch_gallery_unlocker.hpp"

extern int emulated_gbs;

game_and_watch_gallery_unlocker::game_and_watch_gallery_unlocker(std::vector<gb*> g_gb) {
	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));
}


byte game_and_watch_gallery_unlocker::receive_from_linkcable(byte data) {

	//for now, try to brutforce this 
	byte out = ((counter++ % 2) == 0) ? 0xEE : out_data++;

	v_gb[0]->get_cpu()->log_link_traffic(data, out);

	return out;
		
}

