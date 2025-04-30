/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Barcode Taisen Bardigun Emulation
   Copyright (C) 2024  Tim Oelrichs

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


#include "../../gb.h"
#pragma once
#include "../../../libretro/dcgb_hotkey_target.hpp"

#include <vector>
#include <string>
#include <istream>
#include <iostream>
#include <fstream>

class barcode_taisen_bardigun : public I_linkcable_target, public I_dcgb_hotkey_target {

public:
	barcode_taisen_bardigun();
	byte receive_from_linkcable(byte data) override;
	void set_barcode();

	void handle_special_hotkey(int key) override;

	std::vector<byte> data_out; 
	byte current_barcode_index;


};