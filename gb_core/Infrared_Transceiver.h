#pragma once

/*
struct IR_Line
{
    std::deque<Infrared_Signal*> queue;
    Infrared_Signal* current;
    bool has_current = false;
};

class Infrared_Transceiver final : public I_Infrared_Sender, public I_Infrared_Receiver
{

    public:
        Infrared_Transceiver();
        void write_RP_REG(byte value, int cycle);
        byte read_RP_REG(int cycle);
        void receive_edge_ir_signal(Infrared_Signal* signal, int cycle) override;
        void  receive_ir_pulse(Infrared_Signal* signal) override;
        void set_infrared_target(I_Infrared_Receiver* infrared_target);

    IR_Line rx; //received Infrared Signals
    IR_Line tx; //transmitted Infrared Signals

    private:

        Rp_Reg_Bitfield rp_reg; // GBC Infraredreg RP (0xFF56:)
        const int START_DELAY = 16; const int RISE_DELAY = 8;
        const int FALL_DELAY = 24;



        I_Infrared_Receiver* infrared_target;
        bool rp_reg_read_is_enabled() const;
        void update_rx(int now);
        void send_ir_signal(Infrared_Signal* signal) override;
        void correct_prev_duration(IR_Line* line, int new_start, Rp_Reg_Bitfield new_ir_state);

        class Infrared_Logger
        {
             public: static void log_ir_traffic_to_file(const Infrared_Signal *signal, bool incoming);
        }logger;
};
*/