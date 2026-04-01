//
// Created by tim on 30.03.26.
//

#include "./gb_core/gb.h"
#include <fstream>
#include "./DoubleCherryEngine/libretro.h"

extern retro_log_printf_t log_cb;
extern bool logging_transfers_to_file_allowed;



void Infrared_Transceiver::write_RP_REG(byte value, int now)
{
		log_cb(RETRO_LOG_INFO, "Enter WRITE IR\n");

		RP_Reg new_ir_state;
		new_ir_state.reg_data = value;
		new_ir_state.set_received_signal(rp_reg.received_signal()); //is READ-ONLY

		/*
		//emulate obscure behavior RP quirk
		if (last_rp_write == 0x00 && dat == 0xC0) {
			if (!new_ir_state.bits.received_signal) new_ir_state.bits.received_signal = 1;
		}
		last_rp_write = dat;
		*/

		if (rp_reg.ir_light_on()!= new_ir_state.ir_light_on())
		{
			log_cb(RETRO_LOG_INFO, "Send IR Signal %d\n",new_ir_state.ir_light_on());

			//add signal to out queu
			send_ir_signal(new Infrared_Signal(new_ir_state.ir_light_on(), 0));

			if (!tx.queue.empty())
			{
				correct_prev_duration(&tx, now, new_ir_state);
				auto& prev_signal = tx.queue.back();
				Infrared_Transceiver::Infrared_Logger::log_ir_traffic_to_file(prev_signal, true);
			}

		}

		rp_reg.reg_data = new_ir_state.reg_data;
}


byte Infrared_Transceiver::read_RP_REG(int cycle)
{
			log_cb(RETRO_LOG_INFO, "Enter READ IR Signal\n");

			if (rp_reg.read_enabled())
			{
				log_cb(RETRO_LOG_INFO, "Enter READ IR Signal is enabled\n");
				update_rx(cycle);
				log_cb(RETRO_LOG_INFO, "Update rx done\n");
				bool level = false;
				if (rx.has_current) {
					log_cb(RETRO_LOG_INFO, "has current signal\n");
					level =  !rx.current->light_on;
				}
				else
					log_cb(RETRO_LOG_INFO, "has no current signal\n");

				log_cb(RETRO_LOG_INFO, "Read set Signal %d\n", level);
				rp_reg.set_received_signal(!level);

				return rp_reg.reg_data | 0x3C;
			}
			else
				return rp_reg.reg_data | 0x3E;

}

void Infrared_Transceiver::receive_edge_ir_signal(Infrared_Signal* new_signal, int cycle)
{
	int new_start = cycle + START_DELAY;
	new_signal->start_cycle = new_start;
	new_signal->duration = 0;

	if (!rx.queue.empty())
	{
		RP_Reg new_ir_state;
		new_ir_state.reg_data = rp_reg.reg_data;
		new_ir_state.set_received_signal(!new_signal->light_on);

		correct_prev_duration(&rx, new_start, new_ir_state);
		auto& prev_signal = rx.queue.back();
		Infrared_Transceiver::Infrared_Logger::log_ir_traffic_to_file(prev_signal, true);
	}

	rx.queue.emplace_back(new_signal);
}

void Infrared_Transceiver::receive_ir_pulse(Infrared_Signal* new_signal)
{
	rx.queue.emplace_back(new_signal);
}




void Infrared_Transceiver::correct_prev_duration(IR_Line* ir_line, int new_start, RP_Reg new_ir_state)
{
	if (ir_line->queue.empty()) return;
	auto& prev_signal = ir_line->queue.back();

	//correct prev_signal duration
	int prev_end = new_start;
	prev_signal->duration = prev_end - prev_signal->start_cycle;

	if (prev_signal->light_on == true && new_ir_state.received_signal() == false)
		prev_signal->duration += FALL_DELAY;
	else if (prev_signal->light_on == false && new_ir_state.received_signal() == true)
		prev_signal->duration += RISE_DELAY;

	// 🔥 correct duration conflicts
	int corrected_prev_end = prev_signal->start_cycle + prev_signal->duration;

	if (corrected_prev_end > new_start)
		prev_signal->duration = std::max(0, new_start - prev_signal->start_cycle);

}

void Infrared_Transceiver::update_rx(int now)
{

	if (rx.has_current)
	{
		int end = rx.current->start_cycle + rx.current->duration;

		if (now >= end)
		{
			rx.has_current = false;
		}
	}

	while (!rx.has_current && !rx.queue.empty())
	{
		auto next = rx.queue.front();

		if (now < next->start_cycle)
			break;

		rx.queue.pop_front();

		int end = next->start_cycle + next->duration;

		if (now >= end && next->duration != 0)
		{
			continue;
		}

		rx.current = next;
		rx.has_current = true;
	}
}

void Infrared_Transceiver::send_ir_signal(Infrared_Signal* signal)
{
	if (infrared_target)  infrared_target->receive_edge_ir_signal(signal, 0);
	else
		log_cb(RETRO_LOG_INFO, "No linked IR Targetl\n");
}

void Infrared_Transceiver::Infrared_Logger::log_ir_traffic_to_file(const Infrared_Signal* signal, bool incoming)
{
	if (logging_transfers_to_file_allowed)
	{
		std::string filePath = "./ir_logger.txt";
		std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

		// Dauer in Mikrosekunden berechnen
		double duration_us = signal->duration * (1'000'000.0 / 4'194'304.0);

		if (incoming)
			ofs << "\t\t\t\t\t\t\t\t\t\t\t<" << signal->light_on << "\t" << signal->duration
				<< " (" << duration_us << " us)";
		else
			ofs << ">" << signal->light_on << "\t" << signal->duration
				<< " (" << duration_us << " us)";

		ofs << std::endl;
		ofs.close();

	}
}

void Infrared_Transceiver::serialize(serializer &s)
{
	s_VAR(rp_reg.reg_data);
}