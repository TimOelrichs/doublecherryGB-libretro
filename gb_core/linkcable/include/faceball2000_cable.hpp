#include "../../gb.h"

class faceball2000_cable final : public I_linkcable_target {

public:
	faceball2000_cable(std::vector<gb*> g_gb);
	byte receive_from_linkcable(byte data) override;

private:
	std::vector<gb*> v_gb;

};