#include "../gb_core/linkcable/include/link_master_device.hpp"
#include "../NetPacketReceiveHandler.h"
#include "../NetPacketSendHandler.h"
#include "../NetPacketManager.h"


enum PokemonTcg_transfer_state
{
    IDLE,
    DATA
};


class PokemonTcgNetpacketHandler final :
    public link_master_device,
    public  NetPacketReceiveHandler,
    public  NetPacketSendHandler
{
public:

    void save_state_mem(void* buf) override {};
    void restore_state_mem(void* buf) override{};
    void serialize(serializer& s) override {};
    void process() override;
    void reset() override{};

    void handleReceive(const void* buf, std::size_t len, uint16_t client_id) override;
    void handleSend(uint16_t client_id, const void* buf, size_t len) override;

    void setIdleMode(bool enabled)
    {
        is_in_idle_mode = enabled;
        NetpacketManager::getInstance().lockstep_mode_enabled = is_in_idle_mode;
    }

private:
    bool is_in_idle_mode = false;
    uint32_t seri_occer = 0;
    const uint32_t transfer_interval = 17328;

    int receiver_id = 0;
    bool got_receiver_id = false;
};

