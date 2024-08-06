#include "link_master_device.hpp"

enum hack_4p_tetris_state
{
	TITLE_SCREEN,
	MUSIC_SELECT,
	HEIGHT_SELECT,
	IN_GAME,
	WINNER_SCREEN,
	START_NEXT
};

enum hack_4p_tetris_player_state
{
	IS_ALIVE,
	IS_KO,
	IS_SENDING_LINES,
	IS_WINNER,
	IS_LOOSER,
	IS_IN_WINNER_SCREEN

};

struct hack_4p_tetris_lines_packet {
	byte lines;
	int from;
};

/*
struct hack_4p_tetris_mem_state {

	int transfer_speed = 512 * 8;
	int seri_occer = 2048 * 2048 * 2048;

	byte in_data_buffer[4];
	std::queue<byte> out_height_blocks;
	std::queue<byte> out_falling_blocks;
	std::queue<byte> send_data_vec;
	std::queue<hack_4p_tetris_lines_packet> lines_vec;

	byte falling_block_choice[10] = {
		0x00,
		0x04,
		0x04,
		0x08,
		0x08,
		0x0c,
		0x0c,
		0x10,
		0x14,
		0x18
	};

	hack_4p_tetris_state tetris_state = TITLE_SCREEN;
	hack_4p_tetris_player_state players_state[4];
	byte current_max_height = 0;
	int lines_from_player_id = 0;
	byte lines_to_send[4] = { 0,0,0,0 };
	byte next_bytes_to_send[4] = { 0,0,0,0 };
	int process_counter = 0;
	int process_occer = 5;

	// This method lets cereal know which data members to serialize
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			in_data_buffer,
			out_height_blocks,
			out_falling_blocks,
			send_data_vec,
			lines_vec,
			falling_block_choice,
			tetris_state,
			players_state,
			current_max_height,
			lines_from_player_id,
			lines_to_send,
			next_bytes_to_send,
			process_counter,
			process_occer
		); // serialize things by passing them to the archive
	}

};
*/

class hack_4p_tetris : public link_master_device {

	friend class cpu;
public:

	hack_4p_tetris(std::vector<gb*> g_gb);

	void process() override;
	void reset() override;
	
	void save_state_mem(void* buf) override;
	void restore_state_mem(void* buf) override;
	size_t get_state_size() override;
	
	void serialize(serializer& s) override;


	//bool is_ready_to_process();

private:
	void log_traffic(byte id, byte b);
	void init_send_data_vec();
	void clear_data_for_next_round();

	void generate_height_blocks();
	void generate_falling_blocks();
	void hard_reset();
	/*
	void get_all_SC_reg_data();
	void get_all_SB_reg_data();
	bool is_expected_data(byte data);
	bool all_IE_are_handled();

	void broadcast_byte(byte data);
	void send_byte(byte which, byte data);
	*/

	void send_ingame_bytes();
	void handle_ingame_data();
	void update_ingame_states();
	int player_alive_count(); 
	int check_winner_id();
	bool all_are_in_winnerscreen();
	bool winner_winner_chicken_dinner();

	/*
	std::vector<gb*> v_gb;
	hack_4p_tetris_mem_state mem{};

	int transfer_speed = 512 * 8;
	int seri_occer = 2048 * 2048 * 2048;
	*/

	//byte in_data_buffer[4];
	std::vector<byte> out_height_blocks;
	std::vector<byte> out_falling_blocks;
	std::vector<byte> send_data_vec;
	std::vector<hack_4p_tetris_lines_packet> lines_vec;

	byte falling_block_choice[10] = {
		0x00,
		0x04,
		0x04,
		0x08,
		0x08,
		0x0c,
		0x0c,
		0x10,
		0x14,
		0x18
	};

	hack_4p_tetris_state tetris_state;
	hack_4p_tetris_player_state players_state[16];
	byte current_max_height;
	int lines_from_player_id;
	byte lines_to_send[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	byte next_bytes_to_send[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	byte win_counter[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int process_counter;
	//int process_occer;

};
