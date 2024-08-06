#include "link_master_device.hpp"

enum hack_4p_burger_time_deluxe_game_state
{
	BURGERTIME_TITLE_SCREEN,
	BURGERTIME_HANDYCAP_SELECTION,
	BURGERTIME_START_MATCH,
	BURGERTIME_IN_GAME,
	BURGERTIME_CLEAR_SCREEN,
	BURGERTIME_START_NEXT
};

class hack_4p_burger_time_deluxe : public link_master_device {

	friend class cpu;
public:

	hack_4p_burger_time_deluxe(std::vector<gb*> g_gb);

	void process();
	bool is_ready_to_process();
	void reset();
	/*
	void save_mem_state(void* buf);
	void restore_mem_state(void* buf);
	size_t get_state_size();
	*/
	void serialize(serializer& s) override;
	
private:
	void log_traffic(byte id, byte b);
	void fill_send_data_queue(char sequence[]);
	void init_send_data_queue();
	void clear_data_for_next_round();

	void get_all_SC_reg_data();
	void get_all_SB_reg_data();
	bool is_expected_data(byte data);
	bool all_IE_are_handled();

	void broadcast_byte(byte data);
	byte send_byte(byte which, byte data);

	void send_ingame_bytes();
	void handle_ingame_data();
	void update_ingame_states();
	int check_winner_id();
	bool all_are_in_winnerscreen();

	std::vector<gb*> v_gb;
	hack_4p_burger_time_deluxe_game_state game_state;


	// Geerbt über link_master_device
	void save_state_mem(void* buf) override;

	void restore_state_mem(void* buf) override;



};

