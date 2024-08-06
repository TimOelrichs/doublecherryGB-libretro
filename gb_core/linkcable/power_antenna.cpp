/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   Power Antenna / Bug Sensor Emulation
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

#include "./include/power_antenna.hpp"
#pragma once
#include "../../libretro/libretro.h"

extern retro_set_rumble_state_t rumble_state_cb;
extern retro_set_led_state_t led_state_cb;
extern unsigned int power_antenna_use_rumble;

byte power_antenna::receive_from_linkcable(byte in_data)
{
	if (in_data == 0x00) {
		power_antenna_on = false;
		if(rumble_state_cb) rumble_state_cb(0, RETRO_RUMBLE_WEAK, 0);
	}
	else if (!power_antenna_on)
	{
		power_antenna_on = true;
		
	}

	if (power_antenna_last_state != power_antenna_on)
	{
		power_antenna_last_state = power_antenna_on;

		//control LED DRIVER
		led_state_cb(0, power_antenna_on);

		//set rumble
		if(power_antenna_use_rumble)
			rumble_state_cb(0, (retro_rumble_effect)(power_antenna_use_rumble % 2), power_antenna_on * 0xffff);
	}

	return power_antenna_on ? 0xF3 : 0xF2;
}