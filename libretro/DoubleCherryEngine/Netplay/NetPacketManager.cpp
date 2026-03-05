#include "NetPacketManager.h"
#include "../libretro/DoubleCherryEngine/libretro.h"

extern retro_environment_t environ_cb;
// DEFINITION der statischen Callback-Struktur (nur HIER!)
const struct retro_netpacket_callback NetpacketManager::s_iface = {
    NetpacketManager::start_wrapper,     // start
    NetpacketManager::receive_wrapper,   // receive
    NetpacketManager::stop_wrapper,      // stop
    NetpacketManager::poll_receive_wrapper, // poll
    NetpacketManager::connected_wrapper, // connected
    NetpacketManager::disconnected_wrapper, // disconnected
    "DoubleCherryGB netpack V1.0"        // core version char*
};

// Hier kommen dann weitere Methoden-Definitionen, falls nötig
void NetpacketManager::activate_netpacket_api()
{
    if (active_netpacket_api) return;
    environ_cb(RETRO_ENVIRONMENT_SET_NETPACKET_INTERFACE, (void*)&s_iface);
    active_netpacket_api = true;

}
