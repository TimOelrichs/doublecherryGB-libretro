#include "../gb_core/linkcable/include/link_master_device.hpp"
#include "../NetPacketReceiveHandler.h"
#include "../NetPacketSendHandler.h"
#include "../NetPacketManager.h"
#include "gb_core/gb.h"


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
    PokemonTcgNetpacketHandler()
    {
        NetpacketManager::getInstance().lockstep_mode_enabled = true;
    }

    void save_state_mem(void* buf) override {};
    void restore_state_mem(void* buf) override{};
    void serialize(serializer& s) override {};
    void process() override;
    void reset() override{};

    void handleReceive(const void* buf, std::size_t len, uint16_t client_id) override;
    void handleSend(uint16_t client_id, const void* buf, size_t len) override;

    void setIdleMode(bool enabled);
    bool i_am_master();

    gb* gb_instance = nullptr;
private:

    void send_idle_change(bool enabled);
    bool is_in_idle_mode = false;
    int32_t seri_occer=0x7fffffff;;
    const int32_t transfer_interval = 17328;

    int receiver_id = 0;
    bool got_receiver_id = false;

    uint32_t transfer_count = 0;
    uint32_t transfercount_before_ilde_allowed = 130;
    bool idle_master_stopped_by_slave = false;

    byte my_last_send_byte = 0x00;
};

