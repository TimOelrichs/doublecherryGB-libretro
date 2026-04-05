#pragma once
#include "../../gb.h"
#include "./sio_devices.hpp"
#include <time.h>


class LinkCableHUB final : public I_Linkcable_Target {

public:
	LinkCableHUB();
	byte receive_from_linkcable(byte data) override;
	void set_default_link_target(I_Linkcable_Target* default_target) {
		m_default_target = default_target;
	};
	void reset();

private:
	I_Linkcable_Target* m_default_target = nullptr;
	I_Linkcable_Target* m_gb_printer = nullptr;;
	I_Linkcable_Target* m_current_target = nullptr;;

	bool m_first_transfer = true;
	struct timespec m_first_transfer_time = {};
};