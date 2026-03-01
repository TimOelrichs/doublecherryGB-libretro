

#include <bits/types.h>

class NetplayManager
{
    // Singleton
    static NetpacketManager& getInstance() {
        static NetpacketManager instance;
        return instance;
    }




private:
    bool i_am_netplay_client = false;
    bool i_am_netplay_host = false;
    __uint64_t my_random_netplay_id = 0;
};


#endif //DOUBLECHERRYGB_NETPLAYMANAGER_H