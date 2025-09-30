#pragma once
#include "../../gb.h"
#include <vector>

#include <string>
#include <istream>
#include <iostream>
#include <fstream>

enum ubikey_unlocker_state {

	UBIKEY_WAIT_FOR_HELLO,
	UBIKEY_SEND_DATAPACKET,
	UNLOCKER_RECEIVE_DATA,
	UNLOCKER_RECEIVE_DATA_LENGTH

	
};

class ubikey_unlocker final : public I_ir_target, public I_ir_master_device
{

public:

	ubikey_unlocker(std::vector<gb*> gbs);

	void receive_ir_signal(ir_signal* signal) override;
	void send_ir_signal(ir_signal* signal) override;
	void process_ir() override;

	// Geerbt über I_ir_target
	dword* get_rp_que() override;

	void reset() override;

private:

	bool got_hello_msg();
	void check_hello_msg();
	void build_hello_msg();
	void build_ack_msg();
	void build_data_packet();
	int micro_seconds_to_clocks(int micro_seconds) { return (int)(4194304.0 / 1000000.0 * micro_seconds); };
	int clocks_to_micro_seconds(int clocks) { return (int)(1000000.0 / 4194304.0 * clocks); };


	void translate_signals_to_bytes();

	void add_byte_to_out_ir_signals(byte data);
	void add_preamble_to_out_signals();
	void add_postamble_to_out_signals();

	word calc_checksum();

	std::vector<gb*> v_gb;

	ubikey_unlocker_state current_state;
	std::vector<ir_signal*> in_ir_signals, out_ir_signals;
	std::vector<byte> in_bytes, bytes_out_for_msg, all_bytes_out_for_checksum;
	int sending_delay;

	byte in_data_length;
	bool is_master;
	int hello_counter = 0;


	void log_ir_traffic(ir_signal* signal, bool incoming);
	void log_ir_received_bytes();
	void log_ir_answer_delay();

	bool log_answer_delay;
	int clock_at_last_sended_signal;



};