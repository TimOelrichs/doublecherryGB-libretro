#pragma once
#include "../../gb.h"
#include "../../../libretro/dcgb_hotkey_target.hpp"


enum class Alleyway_Controls_Mode {
	NORMAL,
	ANALOG_STICK,
	MOUSE,
	TOUCHSCREEN,
	COUNT
};



class alleyway_link_controller final : public I_linkcable_target, public I_dcgb_hotkey_target
{

public:
	alleyway_link_controller() {};
	byte receive_from_linkcable(byte data) override;
	void handle_special_hotkey(int key) override;

private:
	byte get_paddle_value(unsigned port);
	uint8_t get_paddle_from_mouse(unsigned port);

	bool is_even_transfer = true;
	bool use_mouse_controls = true; 
	

};