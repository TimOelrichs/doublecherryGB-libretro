#include "./include/linkcable_hub.hpp"

extern bool get_monotonic_time(struct timespec* ts);

LinkCableHUB::LinkCableHUB() {
	m_gb_printer = new gameboy_printer();
	reset();
};


void LinkCableHUB::reset() {
	m_first_transfer = true; 
	m_first_transfer_time = {};
	m_current_target = nullptr;
}

byte LinkCableHUB::receive_from_linkcable(byte data) {

	struct timespec current_time;
	get_monotonic_time(&current_time);

	if (!m_first_transfer) {

		long elapsed_s = (current_time.tv_sec - m_first_transfer_time.tv_sec);

		if (elapsed_s >= 15) {
			reset();
		}
	}


	if (m_first_transfer) {
		switch (data)
		{
		case 0x88: //GB_Printer
			m_current_target = m_gb_printer; break;
		default:
			break;
		}

		m_first_transfer = false;
		m_first_transfer_time = current_time;
	}

	if (m_current_target) return m_current_target->receive_from_linkcable(data);
	if (m_default_target) return m_default_target->receive_from_linkcable(data);
	
	//no connection
	return 0xFF;
		

}