#ifndef IG20240316224041
#define IG20240316224041

#include <cstddef>

class Attack
{
public:
    size_t range{};
    int damage{};

    Attack(size_t range, int damage)
        : range(range)
        , damage(damage)
    {
    }
};

#endif