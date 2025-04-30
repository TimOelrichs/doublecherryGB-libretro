#include "../../gb.h"
#pragma once
#include "../../../libretro/dcgb_hotkey_target.hpp"

class alleyway_link_controller : public I_linkcable_target, public I_dcgb_hotkey_target
{

public:
	alleyway_link_controller() {};
	byte receive_from_linkcable(byte data) override;
	void handle_special_hotkey(int key) override;

private:
	byte get_paddle_value(unsigned port);
	bool is_even_transfer = true;


};