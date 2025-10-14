#pragma once
#include "link_master_device.hpp"
#include "../../serializer.h"


constexpr byte PING_SIGNAL = 0x88;
constexpr byte SYNC_SIGNAL = 0xAA;
constexpr byte RESET_SIGNAL = 0xFF;


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

class dmg07 final : public link_master_device {

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

protected:
	void update_seri_occer() override;
private:
	/*
	bool is_ready_to_process();
	void get_all_SC_reg_data();
	void get_all_SB_reg_data();
	bool is_expected_data(byte data);
	bool all_IE_are_handled();
	*/

	void handle_answer(int i, byte dat);
	void handle_answer_pingphase(int i, byte dat);
	void handle_answer_for_restart_pingphase(int i, byte dat);
	void restart_pingphase();
	void log_traffic(byte id, byte b);

	
	byte send_byte(byte which, byte dat) override;
	// void broadcast_byte(byte dat);
	
	void send_sync_bytes();
	void send_each_ping_byte();
	byte send_ping_byte(byte which);
	void process_transmission_phase();

	void fill_buffer_for_less_than_4p();
	void clear_all_buffers();

	int microseconds_to_clocks(int microseconds) {
		return (int)((microseconds * 4.194304f) + 0.5f);
	}; 




	std::vector<byte> trans_buffer[4];
	std::vector<byte> answerbytes_buffer[4];
	std::vector<byte> bytes_to_send;

	dmg07_state current_state = PING_PHASE;

	const int transfer_time_per_byte_in_clocks = microseconds_to_clocks(128);
	const int delay_between_bytes_in_clocks_default = microseconds_to_clocks(2457);
	const int delay_between_packages_in_clocks_default = microseconds_to_clocks(12290);
	const int transmission_base_delay_between_packages_in_ms = 138656;

	int last_trans_nr[4];
	int delay_between_bytes_in_clocks = delay_between_bytes_in_clocks_default;
	int delay_between_packages_in_clocks = delay_between_packages_in_clocks_default;

	int last_log_clock = 0;


	int transfer_count;
	int phase_byte_count;

	
	int restart_in;
	int first_aa_trans_nr;
	int sync_trans_nr;

	int delay;
	
	byte enter_status;

	byte current_packet_size = 1;
	byte packet_size = 0;
	byte transfer_rate;

	
	bool ready_to_sync_master;
	bool master_is_synced;
	bool received_pingphase_restart_demand = false;
	//byte in_data_buffer[4];


	//std::queue<byte> bytes_to_send;
	//dmg07_mem_state mem{};

	

};

