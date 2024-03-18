#ifndef IG20240317021429
#define IG20240317021429

#include "Singleton.h"
#include <random>

class RNG : public Singleton<RNG>
{
public:
    static inline std::mt19937& rng() { return instance().rng_; }

    static inline void seed(int seed)
    {
        rng().seed(seed);
    }

    static inline unsigned long random()
    {
        return rng()();
    }

    static inline int random(int min, int max)
    {
        std::uniform_int_distribution<int> distribution{min, max};
        return distribution(rng());
    }

private:
    static inline std::random_device rd_{};
    static inline std::seed_seq ss_{rd_(), rd_(), rd_(), rd_(), rd_(), rd_(), rd_(), rd_()}; // get 8 integers of random numbers from std::random_device for our seed
    static inline std::mt19937 rng_{ss_};
};

#endif