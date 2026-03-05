#pragma once
#include <memory>

#include "NetPacketReceiveHandler.h"
#include "../libretro.h"
#include "../common/interfaces/ISingleton.hpp"


class NetpacketManager final : public ISingleton<NetpacketManager> {
    friend class ISingleton<NetpacketManager>;
public:

    void activate_netpacket_api();
    [[nodiscard]] bool netpacket_is_active() const {return active_netpacket_api;}

    void setReceiveHandler(std::unique_ptr<NetPacketReceiveHandler> handler) {
        m_receive_handler = std::move(handler);
    }

    void poll_receive() {
        if (m_pollrcv_fn_ptr)
            m_pollrcv_fn_ptr();
    }
    
    void send(uint16_t client_id, const void* buf, size_t len) {
        if (m_send_fn_ptr)
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
    
    // Callback-struct for RetroArch
    static const struct retro_netpacket_callback s_iface;

private:

    NetpacketManager() = default;
    ~NetpacketManager() = default;

    NetpacketManager(const NetpacketManager&) = delete;
    NetpacketManager& operator=(const NetpacketManager&) = delete;

    void start(uint16_t client_id, retro_netpacket_send_t send_fn, 
               retro_netpacket_poll_receive_t poll_receive_fn) {
        m_send_fn_ptr = send_fn;
        m_pollrcv_fn_ptr = poll_receive_fn;
        m_num_clients = 0;
        m_my_client_id = client_id;
    }
    
    void stop() {
        m_send_fn_ptr = nullptr;
        m_pollrcv_fn_ptr = nullptr;
    }

    void receive(const void* buf, size_t len, uint16_t client_id) {
        if (m_receive_handler) {
            m_receive_handler->handleReceive(buf, len, client_id);
        }
    }

    // Statische Wrapper für Callbacks (müssen statisch sein für C-API)
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
    bool waiting_for_netpacket = false;

    std::queue<byte> received_netpacket_data;
    std::unique_ptr<NetPacketReceiveHandler> m_receive_handler;
    retro_netpacket_send_t m_send_fn_ptr = nullptr;
    retro_netpacket_poll_receive_t m_pollrcv_fn_ptr = nullptr;

};

// Definition der Callback-Struktur (mit Wrappern)
const struct retro_netpacket_callback NetpacketManager::s_iface = {
    NetpacketManager::start_wrapper,     // start
    NetpacketManager::receive_wrapper,   // receive
    NetpacketManager::stop_wrapper,      // stop
    NetpacketManager::poll_receive_wrapper, // poll
    NetpacketManager::connected_wrapper, // connected
    NetpacketManager::disconnected_wrapper, // disconnected
    "DoubleCherryGB netpack V1.0",       // core version char*
};