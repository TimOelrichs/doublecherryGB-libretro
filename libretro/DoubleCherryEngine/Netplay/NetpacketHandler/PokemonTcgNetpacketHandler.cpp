//
// Created by tim on 02.03.26.
//

#include "PokemonTcgNetpacketHandler.h"
#include "../NetPacketManager.h"
#include "../../../../gb_core/gb.h"

extern retro_log_printf_t log_cb;
extern std::vector<gb*> v_gb;

const byte LINKCABLE_SPEED_CHANGE = 0x01;
const byte IDLE_MAGIC_NUMBER = 0xAC;


//Fake Master GB when instance is slave and in idle-mode
void PokemonTcgNetpacketHandler::process()
{
    if (!is_in_idle_mode) {
        return;
    }
    log_cb(RETRO_LOG_INFO, "entered master process \n");
    const bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 1;
    uint32_t now = v_gb[0]->get_cpu()->get_clock();
    const bool reached_next_transfer_clock = (now - seri_occer) >= transfer_interval;

    if (isMaster || !reached_next_transfer_clock ) {
        log_cb(RETRO_LOG_INFO, "master process return early\n");
        return;
    }
    log_cb(RETRO_LOG_INFO, "master process actually needs to do something\n");
    //Fake Idle
    byte answer[] = {v_gb[0]->receive_from_linkcable(IDLE_MAGIC_NUMBER)};

    if (answer[0] != IDLE_MAGIC_NUMBER)
    {
        NetpacketManager& netpacket_manager = NetpacketManager::getInstance();
        netpacket_manager.send(receiver_id, answer, 1);
        setIdleMode(false);
    }
    else
        this->seri_occer = now + transfer_interval;

}

void PokemonTcgNetpacketHandler::handleSend(uint16_t client_id, const void* buf, size_t len)
{
    bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 1;
    if (isMaster && is_in_idle_mode)
    {
        //Fake IDLE TRANSFER to improve performance
        NetpacketManager::getInstance().received_netpacket_data.push(IDLE_MAGIC_NUMBER);
        return;
    }
    if (m_send_fn_ptr)
        m_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, buf, len, client_id);
}

void PokemonTcgNetpacketHandler::handleReceive(const void* buf, std::size_t len, uint16_t client_id)
{
    if (!got_receiver_id)
    {
        got_receiver_id = true;
        receiver_id = client_id;
    }

    NetpacketManager& netpacket_manager = NetpacketManager::getInstance();

    switch (len)
    {
    case 1: //normal data byte
        {
            const byte* data = reinterpret_cast<const byte*>(buf);

            bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 1;
            if (!isMaster) {

                if (!is_in_idle_mode)
                {
                    byte answer_data[1] =  {v_gb[0]->receive_from_linkcable(data[0])};
                    netpacket_manager.send(client_id, answer_data, 1);

                    if (data[0] == IDLE_MAGIC_NUMBER)
                    {
                        setIdleMode(true);
                        return;
                    }

                    v_gb[0]->get_cpu()->seri_occer = v_gb[0]->get_cpu()->total_clock + 17328;
                    return;
                }

                if (data[0] != IDLE_MAGIC_NUMBER)
                {
                    setIdleMode(false);
                    byte answer_data[1] =  {v_gb[0]->receive_from_linkcable(data[0])};
                    netpacket_manager.send(client_id, answer_data, 1);

                    v_gb[0]->get_cpu()->seri_occer = v_gb[0]->get_cpu()->total_clock + 17328;

                }

            }

            //Master
            if ( is_in_idle_mode && data[0] != IDLE_MAGIC_NUMBER)
                setIdleMode(false);

            netpacket_manager.received_netpacket_data.push(data[0]);
            netpacket_manager.waiting_for_netpacket = false;
            return;
        }

    case 2:
        {
            /*
            byte command_byte = &buf[0];
            byte command_data =  &buf[1];

            switch (command_byte)
            {
                case LINKCABLE_SPEED_CHANGE:
                {

                }

            }
            */
        }

    }


}

