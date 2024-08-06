/*--------------------------------------------------

   DoubleCherryGB - Gameboy Emulator (based on TGBDual)
   IR TV REMOTE Emulation
   Copyright (C) 2023  Tim Oelrichs

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
   Status: WIP - not working yet
   haven't record the ir data sucessfully, receiver GB doesn't send ACK, but i guess it should
   needs more research

*/


#include "./include/ubikey_unlocker.hpp"


ubikey_unlocker::ubikey_unlocker(std::vector<gb*> gbs)
{

	v_gb.insert(v_gb.begin(), std::begin(gbs), std::end(gbs));
	reset();

	if (is_master)
	{
		for (int i = 0; i < 4; i++)
		{
			build_hello_msg();
		}

		out_ir_signals.push_back(new ir_signal(1, 472));
		out_ir_signals.push_back(new ir_signal(0, 472));
		out_ir_signals.push_back(new ir_signal(1, 472));
		out_ir_signals.push_back(new ir_signal(0, 1064024));
		
	}
	   
}

void ubikey_unlocker::receive_ir_signal(ir_signal* signal)
{

	out_ir_signals.push_back(signal);
	log_ir_traffic(signal, true);
	if (log_answer_delay) log_ir_answer_delay();

	if (is_master)
	{
		is_master = false; 
		switch (current_state)
		{
		case UNLOCKER_RECEIVE_DATA:
		{
			/*
			if (in_ir_signals.size() == 100)
			{
				if (got_hello_msg())
				{
					//ignore hello and send delayed hello, this will set the GBC in slave mode
					build_hello_msg();
					//current_state = WAIT_FOR_HELLO;
					sending_delay = v_gb[0]->get_cpu()->get_clock() + micro_seconds_to_clocks(10000);

				}

				in_ir_signals.clear();
			}
			break;
			*/
		}
		/*
		case WAIT_FOR_HELLO:
		{
			if (in_ir_signals.size() == 3)
			{
				if (got_hello_msg())
				{

					//BUILD LENGTH MSG
					bytes_out_for_msg.push_back(0x5A);
					bytes_out_for_msg.push_back(0x01);
					build_data_msg(bytes_out_for_msg);

					all_bytes_out_for_checksum.insert(all_bytes_out_for_checksum.end(), bytes_out_for_msg.begin(), bytes_out_for_msg.end());
					bytes_out_for_msg.clear();

					//BUILD DATA MSG
					bytes_out_for_msg.push_back(0x96);
					build_data_msg(bytes_out_for_msg);

					all_bytes_out_for_checksum.insert(all_bytes_out_for_checksum.end(), bytes_out_for_msg.begin(), bytes_out_for_msg.end());
					bytes_out_for_msg.clear();

					//BUILD CHECKSUM
					word checksum = calc_checksum();
					byte low = (byte)checksum;
					byte high = (byte)(checksum >> 8);
					bytes_out_for_msg.push_back(low);
					bytes_out_for_msg.push_back(high);
					build_data_msg(bytes_out_for_msg);
					bytes_out_for_msg.clear();


					//current_state = WAIT_FOR_ACK;
					sending_delay = v_gb[0]->get_cpu()->get_clock() + micro_seconds_to_clocks(1000);


				}

				in_ir_signals.clear();
			}
			break;
		}
		case WAIT_FOR_ACK:
		{
			break;
		}

		}
	}

	*/
		}
	}
	//i implemented SLAVE MODE is only for research purposes
	//in prod it will always acts as a master cause this make bypassing the rigion-lock possible
	else
	{
		switch (current_state)
		{
		case UNLOCKER_RECEIVE_DATA:
		{
			/*
			if (in_ir_signals.size() == 100)
			{
				if (got_hello_msg())
				{
					build_hello_msg();
					//current_state = RECEIVE_DATA_LENGTH;
					sending_delay = v_gb[0]->get_cpu()->get_clock() + micro_seconds_to_clocks(1000);

				}

				in_ir_signals.clear();
			}
			*/
			break;
		}
		/*
		case RECEIVE_DATA_LENGTH:
		{
			//2 Byte -> (16 Bits * 2 Signals each) + 2 PRE AND 2 POSTAMBLE = 36
			if (in_ir_signals.size() == 36)
			{
				//translate_signals_to_bytes();
				//in_data_length = in_bytes[1];
				in_data_length = 1;
				//current_state = RECEIVE_DATA;
				in_ir_signals.clear();
			};

			break;
		}
		case RECEIVE_DATA:
		{
			//DATA LENGTH * 8 Bits * 2 +  2 PRE AND 2 POSTAMBLE
			if (in_ir_signals.size() == ((in_data_length * 8 * 2) + 4))
			{
				//translate_signals_to_bytes();
				//current_state = RECEIVE_DATA_CHECKSUM;
				in_ir_signals.clear();
			}
			break;
		}
		case RECEIVE_DATA_CHECKSUM:
		{
			//DATA LENGTH * 8 Bits * 2 +  2 PRE AND 1 POSTAMBLE (0 not send) = 35
			if (in_ir_signals.size() == 35)
			{
				//translate_signals_to_bytes();
				//current_state = RECEICE_DATA_EMPTY;
				in_ir_signals.clear();

				build_ack_msg();

			}
			break;
		}
		case RECEICE_DATA_EMPTY:
		*/
		default:
			break;
		}

	}


}

void ubikey_unlocker::send_ir_signal(ir_signal* signal)
{
	v_gb[0]->receive_ir_signal(signal);
}

void ubikey_unlocker::process_ir()
{
	if (!is_master) return;

	if (out_ir_signals.empty()) return;
	//if (v_gb[0]->get_cpu()->get_clock() < sending_delay) return;

	send_ir_signal(out_ir_signals[0]);

	out_ir_signals.erase(out_ir_signals.begin());


	//log answer delay for reseach purpose
	if (out_ir_signals.empty())
	{
		if(hello_counter++ < 195)	build_hello_msg();
	}


}

void ubikey_unlocker::build_hello_msg()
{
	out_ir_signals.push_back(new ir_signal(1, 550));
	out_ir_signals.push_back(new ir_signal(0, 472));
	out_ir_signals.push_back(new ir_signal(1, 550));
	out_ir_signals.push_back(new ir_signal(0, 11540));

}


void ubikey_unlocker::reset()
{
	sending_delay = 0;
	sending_delay = 0;
	in_data_length = 0;
	//current_state = RECEIVE_HELLO;
	in_ir_signals.clear();
	out_ir_signals.clear();
	in_bytes.clear();
	is_master = true;
	log_answer_delay = true;
	clock_at_last_sended_signal = 0;

}


void ubikey_unlocker::log_ir_traffic(ir_signal* signal, bool incoming) {

	//if (logging_allowed)
	{
		std::string filePath = "./ir_pika_log.txt";
		std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);


		//ofs << "" << clocks_occer << tabs;
		//ofs << "" << std::hex << (int)a << "\t";
		//ofs << "" << std::hex << (int)b << "";

		//ofs << "" << (int)signal->light_on << "\t";
		if (incoming) ofs << "<" << signal->duration;
		else ofs << ">" << signal->duration;

		ofs << std::endl;
		ofs.close();

	}
}

void ubikey_unlocker::log_ir_received_bytes()
{
	std::string filePath = "./ir_bytes.txt";
	std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

	for (int i = 0; i < in_bytes.size(); i++)
	{
		ofs << std::hex << in_bytes[i];
		ofs << std::endl;
	}

	ofs.close();
}

void ubikey_unlocker::log_ir_answer_delay()
{
	//if (logging_allowed)
	{
		std::string filePath = "./ir_logger.txt";
		std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);

		ofs << "ANSWER DELAY: " << (v_gb[0]->get_cpu()->get_clock() - clock_at_last_sended_signal);

		ofs << std::endl;
		ofs.close();

		clock_at_last_sended_signal = 0;
		log_answer_delay = false;

	}
}





//delete? ererbt von pickachu
//
//


bool ubikey_unlocker::got_hello_msg()
{
	return 	(
		in_ir_signals[0]->light_on &&
		!in_ir_signals[1]->light_on &&
		in_ir_signals[2]->light_on &&
		in_ir_signals[0]->duration < 650 &&
		in_ir_signals[1]->duration > 1600 &&
		in_ir_signals[2]->duration < 650
		);
}

void ubikey_unlocker::check_hello_msg()
{
	/*
	* //moved cause i need variations of the state and sending_delay
	if (got_hello_msg())
	{
		build_hello_msg();
		//current_state = RECEIVE_DATA_LENGTH;
		sending_delay = v_gb[0]->get_cpu()->get_clock() + micro_seconds_to_clocks(1000);

	}

	in_ir_signals.clear();
	*/
}




void ubikey_unlocker::build_ack_msg()
{
	add_preamble_to_out_signals();
	add_nec_byte_to_out_ir_signals(0x6E);
	add_postamble_to_out_signals();

	sending_delay = v_gb[0]->get_cpu()->get_clock() + micro_seconds_to_clocks(23000);
}

void ubikey_unlocker::build_data_msg(std::vector<byte> bytes)
{
	add_preamble_to_out_signals();
	for (int i = 0; i < bytes.size(); i++)
	{
		add_nec_byte_to_out_ir_signals(bytes[i]);
	}
	add_postamble_to_out_signals();

	sending_delay = v_gb[0]->get_cpu()->get_clock() + micro_seconds_to_clocks(3500);
}

//TODO REMOVE from INTERFACE
dword* ubikey_unlocker::get_rp_que() { return 0x0000; }


void ubikey_unlocker::translate_signals_to_bytes() {

	byte received_byte = 0x00;
	int bit_shift_count = 0;

	for (int i = 1; i < in_ir_signals.size(); i++)
	{
		//ignore PRE and POSTAMBLE
		if (in_ir_signals[i]->duration >= micro_seconds_to_clocks(580))
		{
			i++;
			continue;
		}


		received_byte = received_byte << 1;
		// bit 1 if low for 299 microsec, bit 2 if low for 120 mircosec
		received_byte |= (in_ir_signals[i]->duration > micro_seconds_to_clocks(200)) ? 0x01 : 0x00;

		if (++bit_shift_count >= 7)
		{
			in_bytes.push_back(received_byte);
			received_byte = 0x00;
			bit_shift_count = 0;
		}

		i++;

	}

	//log_ir_received_bytes();
}

void ubikey_unlocker::add_nec_byte_to_out_ir_signals(byte data) {

	byte out_bit = data;
	for (int i = 7; i >= 0; i--)
	{
		out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(57)));

		out_bit = ((data >> i) & 0x01);
		int duration = out_bit ? micro_seconds_to_clocks(299) : micro_seconds_to_clocks(120);
		out_ir_signals.push_back(new ir_signal(0, duration));

	}
}

void ubikey_unlocker::add_preamble_to_out_signals() {
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(125)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(660)));
}

void ubikey_unlocker::add_postamble_to_out_signals() {
	out_ir_signals.push_back(new ir_signal(1, micro_seconds_to_clocks(102)));
	out_ir_signals.push_back(new ir_signal(0, micro_seconds_to_clocks(545)));
}

word ubikey_unlocker::calc_checksum()
{
	word checksum = 0;
	for (int i = 0; i < all_bytes_out_for_checksum.size(); i++)
	{
		checksum += all_bytes_out_for_checksum[i];
	}

	all_bytes_out_for_checksum.clear();

	return checksum;
}
