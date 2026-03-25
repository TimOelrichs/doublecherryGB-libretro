
#pragma once
#include <cstdint>
#include "../libretro.h"

class NetpacketManager;

class NetPacketSendHandler {
public:
    virtual ~NetPacketSendHandler() = default;

    virtual void handleSend(uint16_t client_id, const void* buf, size_t len) = 0;
    void set_retro_netpacket_send_t( retro_netpacket_send_t send_fn_ptr) { m_send_fn_ptr = send_fn_ptr; }

protected:
    explicit NetPacketSendHandler() = default;

    retro_netpacket_send_t m_send_fn_ptr = nullptr;
};

class DefaultNetPacketSendHandler final : public NetPacketSendHandler
{
public:
    explicit DefaultNetPacketSendHandler() = default;

    void handleSend(uint16_t client_id, const void* buf, size_t len) override;
};