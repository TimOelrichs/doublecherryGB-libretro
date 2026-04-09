#pragma once
#include <memory>
#include <queue>

#include "../gb_core/gb.h"
#include "NetPacketReceiveHandler.h"
#include "NetPacketSendHandler.h"
#include "../libretro.h"
#include "../common/interfaces/ISingleton.hpp"

extern retro_log_printf_t log_cb;

class NetpacketManager final : public ISingleton<NetpacketManager> {
    friend class ISingleton<NetpacketManager>;
public:

    void activate_netpacket_api();
    [[nodiscard]] bool netpacket_is_active() const
    {
        if(active_netpacket_api)
           return  m_my_client_id == 0 ?  m_num_clients > 0: true;
        return false;
    }

    uint16_t get_netpacket_id() {return m_my_client_id;}

    void setReceiveHandler(std::shared_ptr<NetPacketReceiveHandler> handler) {
        m_receive_handler = std::move(handler);
    }

    void setSendHandler(std::shared_ptr<NetPacketSendHandler> handler) {
        m_send_handler = std::move(handler);
        m_send_handler->set_retro_netpacket_send_t(m_send_fn_ptr);
    }

    void poll_receive() {
        if (m_pollrcv_fn_ptr)
            m_pollrcv_fn_ptr();
    }

    void send(uint16_t client_id, const void* buf, size_t len) {
        //log_cb(RETRO_LOG_INFO, "Send netpacket NManager  \n");
        if (m_send_handler)
        {
           // log_cb(RETRO_LOG_INFO, "call sendhandler  \n");
            m_send_handler->handleSend(client_id, buf, len);
        }
        else if (m_send_fn_ptr)
            m_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, buf, len, client_id);

    }

    bool connected(unsigned short client_id) {
        if (m_num_clients >= m_max_gbs)
            return false;

        m_num_clients++;
        return true;
    }

    void disconnected(unsigned short client_id) {
        m_num_clients--;
    }

    // NUR DEKLARATION, KEINE DEFINITION!
    static const struct retro_netpacket_callback s_iface;

    bool waiting_for_netpacket = false;
    bool lockstep_mode_enabled = false;
    int lock_step_transfer_interval = 17336;
    std::queue<byte> received_netpacket_data;

private:

    NetpacketManager()
    {
        m_receive_handler = std::unique_ptr<NetPacketReceiveHandler>(new DefaultNetPacketReceiveHandler());
        setSendHandler(std::unique_ptr<NetPacketSendHandler>(new DefaultNetPacketSendHandler()));
    }

    ~NetpacketManager() = default;

    NetpacketManager(const NetpacketManager&) = delete;
    NetpacketManager& operator=(const NetpacketManager&) = delete;

    void start(uint16_t client_id, retro_netpacket_send_t send_fn,
               retro_netpacket_poll_receive_t poll_receive_fn) {
        m_send_fn_ptr = send_fn;
        m_pollrcv_fn_ptr = poll_receive_fn;
        m_num_clients = 0;
        m_my_client_id = client_id;
        active_netpacket_api = true;
        m_send_handler->set_retro_netpacket_send_t(send_fn);
    }

    void stop() {
        m_send_fn_ptr = nullptr;
        m_send_handler->set_retro_netpacket_send_t(nullptr);
        m_pollrcv_fn_ptr = nullptr;
    }

    void receive(const void* buf, size_t len, uint16_t client_id) {
        if (m_receive_handler) {
            m_receive_handler->handleReceive(buf, len, client_id);
        }
    }

    // Statische Wrapper für Callbacks
    static void start_wrapper(uint16_t client_id, retro_netpacket_send_t send_fn,
                              retro_netpacket_poll_receive_t poll_receive_fn) {
        getInstance().start(client_id, send_fn, poll_receive_fn);
    }

    static void stop_wrapper() {
        getInstance().stop();
    }

    static void poll_receive_wrapper() {
        getInstance().poll_receive();
    }

    static void receive_wrapper(const void* buf, size_t len, unsigned short client_id) {
        getInstance().receive(buf, len, client_id);
    }

    static bool connected_wrapper(unsigned short client_id) {
        return getInstance().connected(client_id);
    }

    static void disconnected_wrapper(unsigned short client_id) {
        getInstance().disconnected(client_id);
    }

    uint16_t m_my_client_id = 0;
    int m_num_clients = 0;
    const int m_max_gbs = 2;
    bool active_netpacket_api = false;

    std::shared_ptr<NetPacketReceiveHandler> m_receive_handler;
    std::shared_ptr<NetPacketSendHandler> m_send_handler;
    retro_netpacket_send_t m_send_fn_ptr = nullptr;
    retro_netpacket_poll_receive_t m_pollrcv_fn_ptr = nullptr;
};