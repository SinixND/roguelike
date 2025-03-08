#ifndef IG20250309182913
#define IG20250309182913

struct EnergyComponent;

namespace AgilitySystem
{
    [[nodiscard]]
    int increaseAgility(
        int agility,
        EnergyComponent& energyIO,
        int value = 1
    );
}

#endif
