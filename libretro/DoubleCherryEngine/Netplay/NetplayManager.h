

#include <cstdint>
#include <random>
#include <bits/types.h>

class NetplayManager
{
    // Singleton
    static NetplayManager& getInstance() {
        static NetplayManager instance;
        return instance;
    }


private:
    bool i_am_netplay_client = false;
    bool i_am_netplay_host = false;
    uint64_t my_random_netplay_id = 0;

    uint64_t random64bit() {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> dis;

        return dis(gen);
    }
};


#endif //DOUBLECHERRYGB_NETPLAYMANAGER_H