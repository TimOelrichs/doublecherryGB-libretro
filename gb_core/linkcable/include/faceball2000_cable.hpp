#include "../../gb.h"

class faceball2000_cable final : public I_Linkcable_Target {
public:
    faceball2000_cable(const std::vector<gb*>& g_gb);
    byte receive_from_linkcable(byte data) override;

private:
    std::vector<gb*> v_gb;
};