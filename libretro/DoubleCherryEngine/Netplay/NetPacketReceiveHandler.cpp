//
// Created by tim on 02.03.26.
//

#include "NetPacketManager.h"
#include "../../gb_core/gb.h"


extern std::vector<gb*> v_gb;



void DefaultNetPacketReceiveHandler::handleReceive(const void* buf, std::size_t len, uint16_t client_id)
{
    const byte* data = reinterpret_cast<const byte*>(buf);

    bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 1;
    if (!isMaster) {

        byte answer_data[1] =  {v_gb[0]->receive_from_linkcable(data[0])};
        netpacket_manager_.send(client_id, answer_data, 1);
        return;
    }

    netpacket_manager_.received_netpacket_data.push(data[0]);
    netpacket_manager_.waiting_for_netpacket = false;

}

