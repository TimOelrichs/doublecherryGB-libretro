#pragma once
#include "link_master_device.hpp"
#include "../../serializer.h"



//############ DMG-07 

enum dmg07_state
{
	PING_PHASE,
	SYNC_PHASE,
	TRANSMISSION_PHASE,
};

struct dmg07_speed_values {
	int ping_speed;
	int transmission_speed;
};



struct dmg07_mem_state_size {
	size_t size;
	// This method lets cereal know which data members to serialize

	/*
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			size
		); // serialize things by passing them to the archive
	}
	*/

};

class dmg07 : public link_master_device {

	friend class cpu;
public:

	dmg07(std::vector<gb*> g_gb);

	void process() override;
	void reset() override;
	void save_state_mem(void* buf) override;
	void restore_state_mem(void* buf) override;
	size_t get_state_size() override;

	void serialize(serializer& s) override;


	void log_save_state(char* data, size_t size);

private:
	/*
	bool is_ready_to_process();
	void get_all_SC_reg_data();
	void get_all_SB_reg_data();
	bool is_expected_data(byte data);
	bool all_IE_are_handled();
	*/

	void handle_answer(int i, byte dat);
	void restart_pingphase();
	//void log_traffic(byte id, byte b);

	
	byte send_byte(byte which, byte dat) override;
	// void broadcast_byte(byte dat);
	
	void send_sync_bytes();
	void send_each_ping_byte();
	byte send_ping_byte(byte which);

	void fill_buffer_for_less_than_4p();
	void clear_all_buffers();

	dmg07_state current_state = PING_PHASE;

	//converted to no initialize for compat with windows MCSV
	int transfer_count;
	int phase_byte_count;

	int last_trans_nr[4];
	int restart_in;
	byte enter_status;

	byte packet_size;
	byte transfer_rate;

	int first_aa_trans_nr;
	int sync_trans_nr;

	int delay;
	bool ready_to_sync_master;
	bool master_is_synced;

	//byte in_data_buffer[4];

	std::vector<byte> trans_buffer[4];
	std::vector<byte> ans_buffer[4];

	std::vector<byte> bytes_to_send;
	//std::queue<byte> bytes_to_send;
	//dmg07_mem_state mem{};
};

