#include "../../gb.h"

class game_and_watch_gallery_unlocker final : public I_linkcable_target {

public:
	game_and_watch_gallery_unlocker(std::vector<gb*> g_gb);
	byte receive_from_linkcable(byte data) override;

private:
	std::vector<gb*> v_gb;
	byte out_data = 0x01;
	int counter = 0;
};