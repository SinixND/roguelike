#ifndef RANDOMNUMBERGENERATOR_H_20240111104504
#define RANDOMNUMBERGENERATOR_H_20240111104504

#include <random>

namespace snd{
class RNG
{
public:
    static inline void seed(int seed = std::random_device{}())
    {
        rnd(seed);
    }

    static inline int getInt(int from, int to){
        // Create a reusable random number generator that generates uniform numbers
        static inline std::uniform_int_distribution uid{from, to};
        return uid(rnd);
    }

private:
    // this gets created and seeded every time the function is called
    std::mt19937 rnd; 
    };
}

#endif