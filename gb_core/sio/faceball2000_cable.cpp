#include "faceball2000_cable.hpp"

extern int emulated_gbs; 

faceball2000_cable::faceball2000_cable(std::vector<gb*> g_gb) {
	v_gb.insert(v_gb.begin(), std::begin(g_gb), std::end(g_gb));
}


byte faceball2000_cable::seri_send(byte data) {

	byte ret = data; 
	for (size_t i = 1; i < emulated_gbs; i++)
	{
		ret = v_gb[i]->seri_send(ret);
	}
	return ret; 
}



byte faceball2000_cable::get_SB_value() {
	return 0xFF;
}
byte faceball2000_cable::get_SC_value() {
	return 0x80;
}

