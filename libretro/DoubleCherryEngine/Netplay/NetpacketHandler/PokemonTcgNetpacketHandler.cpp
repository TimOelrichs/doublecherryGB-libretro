//
// Created by tim on 02.03.26.
//

#include "PokemonTcgNetpacketHandler.h"
#include "../NetPacketManager.h"
#include "../../../../gb_core/gb.h"

extern link_master_device* master_link;

extern retro_log_printf_t log_cb;

const byte LINKCABLE_SPEED_CHANGE = 0x01;
const byte IDLE_CHANGE_COMMAND = 0x02;
const byte IDLE_MAGIC_NUMBER = 0xAC;



//Fake Master GB when instance is slave and in idle-mode
void PokemonTcgNetpacketHandler::process()
{
    if (!is_in_idle_mode) {
        return;
    }

    const bool isMaster = (gb_instance->get_regs()->SC & 0x01) == 1;

    uint32_t now = gb_instance->get_cpu()->get_clock();


    const bool reached_next_transfer_clock = (gb_instance->get_cpu()->get_clock() - this->seri_occer) > transfer_interval ;

    if (isMaster || !reached_next_transfer_clock ) {
        return;
    }

    this->seri_occer += transfer_interval;

    //Fake Idle
    byte preview_answer[1] = {gb_instance->get_regs()->SB};

    if (preview_answer[0] == IDLE_MAGIC_NUMBER )
        gb_instance->receive_from_linkcable(IDLE_MAGIC_NUMBER);
    else
    {

        NetpacketManager& netpacket_manager = NetpacketManager::getInstance();
        send_idle_change(false);
        log_cb(RETRO_LOG_INFO, "Slave turned Off IDLE mode cause it's got non IDLE Data\n");
        setIdleMode(false);
        master_link = nullptr;
        gb_instance->get_cpu()->seri_occer  = this->seri_occer;

    }



}

void PokemonTcgNetpacketHandler::handleSend(uint16_t client_id, const void* buf, size_t len)
{
    const byte* data = reinterpret_cast<const byte*>(buf);
    if (gb_instance == nullptr) return;

    bool isMaster = (gb_instance->get_regs()->SC & 0x01) == 1;


    if (len == 1)
    {

        if (isMaster)
        {

            this->my_last_send_byte = data[0];
            transfer_count++;
        }


        if (isMaster && is_in_idle_mode && data[0] != IDLE_MAGIC_NUMBER)
        {

            log_cb(RETRO_LOG_INFO, "Master turned off IDLE mode (Data: %02X)\n", data[0]);
            setIdleMode(false);
            send_idle_change(false);

        }

        if (isMaster && is_in_idle_mode && data[0] == IDLE_MAGIC_NUMBER )
        {
            NetpacketManager::getInstance().received_netpacket_data.push(IDLE_MAGIC_NUMBER);
            return; // Netzwerk wird ignoriert
        }


    }

    if (m_send_fn_ptr)
    {
        m_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, buf, len, client_id);
    }
}



void PokemonTcgNetpacketHandler::handleReceive(const void* buf, std::size_t len, uint16_t client_id)
{
    if (!got_receiver_id)
    {
        got_receiver_id = true;
        receiver_id = client_id;
    }

    NetpacketManager& netpacket_manager = NetpacketManager::getInstance();

    const byte* received_data = reinterpret_cast<const byte*>(buf);

    switch (len)
    {
    case 1: //normal data byte
        {

            if (!i_am_master())
            {
                log_cb(RETRO_LOG_INFO, "Slave received netpacket: %d\n", received_data[0]);
                transfer_count++;

                if (!is_in_idle_mode)
                {

                    //byte answer_data[1] =  {gb_instance->receive_from_linkcable(data[0])};
                    byte answer_preview_data[1] =  {gb_instance->get_SB_value()};
                    netpacket_manager.send(client_id, answer_preview_data, 1);

                    bool freshconnection = received_data[0] == 0x29 && answer_preview_data[0] == 0x12;
                    if(freshconnection)    transfer_count = 1;

                    //lockstep, wait for next packet
                    netpacket_manager.received_netpacket_data.push(received_data[0]);
                    gb_instance->get_cpu()->seri_occer = gb_instance->get_cpu()->total_clock - 1;
                    return;
                }

                return;
            }



            //Master should just turn idle by itseld (?)
            log_cb(RETRO_LOG_INFO, "Master Receive Context - Stored: %d, Received: %d, Instance: %p\n", this->my_last_send_byte, received_data[0], (void*)this);
            bool freshconnection = received_data[0] == 0x12 && this->my_last_send_byte == 0x29;
            if(freshconnection)    transfer_count = 1;

            if (!is_in_idle_mode && received_data[0] == IDLE_MAGIC_NUMBER &&  this->my_last_send_byte == IDLE_MAGIC_NUMBER && transfer_count > transfercount_before_ilde_allowed)
            {
                //idle_master_stopped_by_slave = false;
                log_cb(RETRO_LOG_INFO, "Master turned on IDLE mode\n");
                setIdleMode(true);
                send_idle_change(true);

            }

            netpacket_manager.received_netpacket_data.push(received_data[0]);
            netpacket_manager.waiting_for_netpacket = false;
            return;
        }

    case 2:
        {

            const byte command_byte = received_data[0];
            const byte command_data =  received_data[1];

            switch (command_byte)
            {
                case LINKCABLE_SPEED_CHANGE:break; //TODO;
                case IDLE_CHANGE_COMMAND: setIdleMode((bool)command_data); break;
            }

        }

    }


}

void PokemonTcgNetpacketHandler::setIdleMode(bool enabled)
{

    if (!i_am_master())
    {
        if (is_in_idle_mode && !enabled)
        {
            master_link = nullptr;
            gb_instance->get_cpu()->seri_occer  = this->seri_occer;
        }
        if (!is_in_idle_mode && enabled)
        {
            master_link = this;
            uint32_t now = gb_instance->get_cpu()->get_clock();
            this->seri_occer = now + transfer_interval;
        }

    }

    log_cb(RETRO_LOG_INFO, "changed ilde mode: %d\n", enabled);
    is_in_idle_mode = enabled;
    NetpacketManager::getInstance().lockstep_mode_enabled = !is_in_idle_mode;

}

bool PokemonTcgNetpacketHandler::i_am_master()
{
    return (gb_instance->get_regs()->SC & 0x01) == 1;
}


void PokemonTcgNetpacketHandler::send_idle_change(bool enabled)
{
    log_cb(RETRO_LOG_INFO, "Send ilde change: %d\n", enabled);
    const byte data[2] = { IDLE_CHANGE_COMMAND, static_cast<byte>(enabled)};

    if (m_send_fn_ptr)
    {
        m_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, data, 2, receiver_id);
    }
}
