#pragma once
#include "../../gb.h"
#include "../../../libretro/dcgb_hotkey_target.hpp"

#include <vector>

#include <string>
#include <istream>
#include <iostream>
#include <fstream>



class full_changer : public I_ir_master_device, public I_dcgb_hotkey_target
{

public:

	full_changer(std::vector<gb*> gbs);

	void send_ir_signal(ir_signal* signal) override;
	void process_ir() override;

	void reset();

	void handle_special_hotkey(int key) override;

private:

	void build_signal(byte cosmic_character_id);
	void add_byte_to_out_ir_signals(byte data);
	int micro_seconds_to_clocks(int micro_seconds) { return (int)(4194304.0 / 1000000.0 * micro_seconds); };
	int clocks_to_micro_seconds(int clocks) { return (int)(1000000.0 / 4194304.0 * clocks); };

	std::vector<gb*> v_gb;
	std::vector<ir_signal*>  out_ir_signals;

	//int short_duration = micro_seconds_to_clocks(125);
	//int long_duration = micro_seconds_to_clocks(250);

};