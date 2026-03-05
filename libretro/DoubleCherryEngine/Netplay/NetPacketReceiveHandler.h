#pragma once
#include <cstdint>


class NetPacketReceiveHandler {
public:
    virtual ~NetPacketReceiveHandler() = default;
    virtual void handleReceive(const void* buf, std::size_t len, uint16_t client_id) = 0;
};


class DefaultNetPacketReceiveHandler : public NetPacketReceiveHandler
{
public:
    void handleReceive(const void* buf, std::size_t len, uint16_t client_id) override;

}

