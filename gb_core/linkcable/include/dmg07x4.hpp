#include "link_master_device.hpp"
#include "dmg07.hpp"

class dmg07x4 : public link_master_device {

public:
	dmg07x4(std::vector<gb*> v_gb, int players);
	void process() override;
	void reset() override;
	void save_state_mem(void* buf) override;
	void restore_state_mem(void* buf) override;
	size_t get_state_size() override;
	void serialize(serializer& s) override;

private:
	std::vector<dmg07*> v_dmg07;
	//int emulated_dmg07s = 2;
};