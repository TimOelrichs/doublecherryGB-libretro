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
    PokemonTcgNetpacketHandler(int treshold, int transfer_interval)
    {
        transfercount_before_ilde_allowed = treshold;
        this->transfer_interval = transfer_interval;
        NetpacketManager::getInstance().lockstep_mode_enabled = true;
        NetpacketManager::getInstance().lock_step_transfer_interval = transfer_interval;
    }

    void save_state_mem(void* buf) override {};
    void restore_state_mem(void* buf) override{};
    void serialize(serializer& s) override {};
    void process() override;
    void reset() override{};

    void handleReceive(const void* buf, std::size_t len, uint16_t client_id) override;
    void handleSend(uint16_t client_id, const void* buf, size_t len) override;

    void setIdleMode(bool enabled);
    void setMagicIdleNumber(uint8_t magic_id){ IDLE_MAGIC_NUMBER = magic_id; };
    void setHandshakeBytes(byte Handshake_Master, byte Handshake_Answer)
    {
        HANDSHAKE_MASTER_BYTE = Handshake_Master;
        HANDSHAKE_ANSWER_BYTE = Handshake_Answer;
    }
    bool i_am_master();

    gb* gb_instance = nullptr;
private:

    bool is_fresh_connection(byte master_data, byte answer_data){ return master_data == HANDSHAKE_MASTER_BYTE &&  answer_data == HANDSHAKE_ANSWER_BYTE;};
    void send_idle_change(bool enabled);
    bool is_in_idle_mode = false;
    int32_t seri_occer=0x7fffffff;;
    int32_t transfer_interval = 34656;

    int receiver_id = 0;
    bool got_receiver_id = false;

    uint32_t transfer_count = 0;
    uint32_t transfercount_before_ilde_allowed = 150;
    bool idle_master_stopped_by_slave = false;

    byte my_last_send_byte = 0x00;

    byte IDLE_MAGIC_NUMBER = 0xAC;

    byte HANDSHAKE_MASTER_BYTE = 0x29;
    byte HANDSHAKE_ANSWER_BYTE = 0x12;
};

