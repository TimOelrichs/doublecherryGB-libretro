#include "NetPacketManager.h"
#include "../libretro/DoubleCherryEngine/libretro.h"

extern retro_environment_t environ_cb;

const struct retro_netpacket_callback NetpacketManager::s_iface = {
    NetpacketManager::start_wrapper,     // start
    NetpacketManager::receive_wrapper,   // receive
    NetpacketManager::stop_wrapper,      // stop
    NetpacketManager::poll_receive_wrapper, // poll
    NetpacketManager::connected_wrapper, // connected
    NetpacketManager::disconnected_wrapper, // disconnected
    "DoubleCherryGB netpack V1.0"        // core version char*
};


void NetpacketManager::activate_netpacket_api()
{
    log_cb(RETRO_LOG_INFO, "Activating Netpacket API\n");
    if (active_netpacket_api) return;
    if (environ_cb)
    {
        environ_cb(RETRO_ENVIRONMENT_SET_NETPACKET_INTERFACE, (void*)&s_iface);
        active_netpacket_api = true;
    }
    log_cb(RETRO_LOG_INFO, "Netpacket API is active\n");
}
