#pragma once
#include <cstdint>

class NetpacketManager;

class NetPacketReceiveHandler {
public:
    virtual ~NetPacketReceiveHandler() = default;
    virtual void handleReceive(const void* buf, std::size_t len, uint16_t client_id) = 0;
protected:
    explicit NetPacketReceiveHandler(NetpacketManager& manager) : netpacket_manager_(manager) {}
    NetpacketManager& netpacket_manager_;
};

class DefaultNetPacketReceiveHandler final : public NetPacketReceiveHandler
{
public:
    explicit DefaultNetPacketReceiveHandler(NetpacketManager& manager) : NetPacketReceiveHandler(manager) {}
    void handleReceive(const void* buf, std::size_t len, uint16_t client_id) override;
};