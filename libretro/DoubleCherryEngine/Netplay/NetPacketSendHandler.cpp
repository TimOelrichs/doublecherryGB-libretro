//
// Created by tim on 24.03.26.
//

#include "NetPacketSendHandler.h"
#include "NetPacketManager.h"
#include "../../../gb_core/gb.h"

extern std::vector<gb*> v_gb;

void DefaultNetPacketSendHandler::handleSend(uint16_t client_id, const void* buf, size_t len)
{
    if (m_send_fn_ptr)
        m_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, buf, len, client_id);
};

