

#include "link_master_device.hpp"
#include <string>

#pragma once
#include "../../libretro/dcgb_hotkey_target.hpp"

class barcodeboy : public I_link_target, public link_master_device, public I_dcgb_hotkey_target, public I_savestate
{
	

public:
	barcodeboy(std::vector<gb*> g_gb, char cart_name[18]);
	bool is_in_mastermode; 
	void init_barcodes(char cart_name[18]);
	void process() override;
	void reset() override;
	byte get_SB_value() override;
	byte get_SC_value() override;
	
	void virtual save_state_mem(void* buf) override;
	void virtual restore_state_mem(void* buf) override;
	size_t virtual get_state_size() override;
	
	void virtual serialize(serializer& s) override;
	
	byte seri_send(byte) override;

	void handle_special_hotkey(int key) override;

private:
	int seri_send_counter; 
	int round; 
	int barcode_index;
	char cart_name[18];
	std::string current_barcode; 
	std::vector<std::string> barcodes; 
};





