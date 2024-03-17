#ifndef IG20240316224041
#define IG20240316224041

#include <cstddef>

struct Attack
{
    size_t range{};
    int damage{};

    Attack(size_t range, int damage)
        : range(range)
        , damage(damage)
    {
    }
};

#endif