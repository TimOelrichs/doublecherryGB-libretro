

#include <cstdint>
#include <random>


class NetplayManager
{

public:
    // Singleton
    static NetplayManager& getInstance() {
        static NetplayManager instance;
        return instance;
    }

    bool netplay_detected() { return i_am_netplay_client || i_am_netplay_host;};
    uint64_t get_netplay_id() { return my_random_netplay_id; }
    void detect_netplay_2player_role(uint64_t host_id)
    {
        i_am_netplay_host = host_id == my_random_netplay_id;
        i_am_netplay_client = !i_am_netplay_host;
    }

    bool i_am_netplay_client = false;
    bool i_am_netplay_host = false;
private:

    uint64_t my_random_netplay_id = 0;

    NetplayManager()
    {
        my_random_netplay_id = random64bit();
    };

    uint64_t random64bit() {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> dis;

        return dis(gen);
    }
};


