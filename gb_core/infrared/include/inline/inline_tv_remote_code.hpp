
//REFACTORING

struct command_code {
	std::string name; 
	int code;
	command_code(std::string name, int code) {
		this->name = name;
		this->code = code;
	}
};
struct predefined_remote {
	tv_remote_protocol protocol;
	byte device_adress;
	std::string remote_name;
	std::vector<command_code*> commands;


	predefined_remote(	tv_remote_protocol protocol,
						byte device_adress, 
						std::string remote_name
						) {
		this->protocol = protocol;
		this->device_adress = device_adress;
		this->remote_name = remote_name; 
	}
};

std::vector<predefined_remote*> remotes;




enum remote_function {
	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	CHANNEL_UP,
	CHANNEL_DOWN,
	VOLUME_UP,
	VOLUME_DOWN,
	POWER,
	TV_VIDEO,
	MUTE,
	AUX,
	TIMER,
	RECALL,
};

std::string remote_function_names[] = {
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"CHANNEL_UP",
	"CHANNEL_DOWN",
	"VOLUME_UP",
	"VOLUME_DOWN",
	"POWER",
	"TV_VIDEO",
	"MUTE",
	"AUX",
	"TIMER",
	"RECALL",
};



byte remote_command_codes[][20] =
{
	{0,1,2,3,8,9,10,11,16,17,18,19,20,21,22,23,24,25,26,27},	//NEC ??? Not sure if correct
	{0,1,2,3,4,5,6,7,8,9,17,18, 13,14,11,15,12,16, 55},			//RC5X Phillips
	{0,1,2,3,4,5,6,7,8,9,17,18, 13,14,11,15,12,16, 55},			//RC6 
	{24,25,26,27,28,29,30,31,32,33,20,21,22,23,1,3,5,17,18},	//JVC
	{0,1,2,3,4,5,6,7,8,9,16,17,18,19,21,47,20,23,47,39},		//SIRC
	{26,17,18,19,20,21,22,23,24,25,8,9,47,48,2,3,7,62,61,59}	//ITT
};

