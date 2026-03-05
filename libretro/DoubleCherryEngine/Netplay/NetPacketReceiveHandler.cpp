//
// Created by tim on 02.03.26.
//

#include "NetPacketReceiveHandler.h"
#include "../../gb_core/gb.h"

void DefaultNetPacketReceiveHandler::handleReceive(const void* buf, std::size_t len, uint16_t client_id)
{
    const byte* data = reinterpret_cast<const byte*>(buf);

    bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 1;
    if (!isMaster) {
        v_gb[0]->receive_from_linkcable(data[0]);
        return;
    }

    v_gb[0]->get_cpu()->received_netpacket_data.push(data[0]);
    v_gb[0]->get_cpu()->waiting_for_netpacket = false;
}
