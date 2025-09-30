

#include "link_master_device.hpp"
#include <string>

#pragma once
#include "../../../libretro/dcgb_hotkey_target.hpp"

class barcodeboy final :  public I_linkcable_target,
					public link_master_device, 
					public I_dcgb_hotkey_target

{
	

public:
	barcodeboy(std::vector<gb*> g_gb, char cart_name[18]);
	bool is_in_mastermode; 
	void init_barcodes(char cart_name[18]);
	void process() override;
	void reset() override;

	void virtual save_state_mem(void* buf) override;
	void virtual restore_state_mem(void* buf) override;
	size_t virtual get_state_size() override;
	
	void virtual serialize(serializer& s) override;
	
	byte receive_from_linkcable(byte) override;

	void handle_special_hotkey(int key) override;

private:
	int in_byte_counter; 
	int round; 
	int barcode_index;
	char cart_name[18];
	std::string current_barcode; 
	std::vector<std::string> barcodes; 
	byte current_barcode_index = 0;
};





