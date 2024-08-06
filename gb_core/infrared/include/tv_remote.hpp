#pragma once
#include "../../gb.h"
#include "../../../libretro/dcgb_hotkey_target.hpp"

#include <vector>

#include <string>
#include <istream>
#include <iostream>
#include <fstream>

enum tv_remote_protocol {
	NEC1,
	RC5X,
	RC6,
	JVC,
	SIRC,
	ITT
};

class tv_remote : public I_ir_master_device, public I_dcgb_hotkey_target
{

public:

	tv_remote(std::vector<gb*> gbs);

	void send_ir_signal(ir_signal* signal) override;
	void process_ir() override;

	void reset();

	void handle_special_hotkey(int key) override;

private:

	void build_predefined_remotes();
	void build_signal_frame(byte code);

	void build_nec_signal_frame(byte adress, byte code);
	void add_nec_byte_to_out_ir_signals(byte data);

	void build_rc5_signal_frame(byte adress, byte command);
	void add_rc5_byte_to_out_ir_signals(byte data, byte length);

	void build_rc6_signal_frame(byte adress, byte command);
	void add_rc6_byte_to_out_ir_signals(byte data);

	void build_sirc_signal_frame(byte adress, byte command);
	void add_sirc_byte_to_out_ir_signals(byte data, byte length);

	void build_jvc_signal_frame(byte adress, byte command);
	void add_jvc_byte_to_out_ir_signals(byte data);

	void build_itt_signal_frame(byte adress, byte command);
	void add_itt_byte_to_out_ir_signals(byte data, byte length);


	void switch_tv_remote();
	std::string get_remote_name();
	byte key_2_remote_command(int key);
	void gen_random_remote_signal();

	int micro_seconds_to_clocks(int micro_seconds) { return (int)(4194304.0 / 1000000.0 * micro_seconds); };
	int clocks_to_micro_seconds(int clocks) { return (int)(1000000.0 / 4194304.0 * clocks); };

	std::vector<gb*> v_gb;

	std::vector<ir_signal*>  out_ir_signals;
	int current_predefined_remote, total_transmission_time, current_remote_protocol, current_device_adress;








};