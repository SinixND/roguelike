#include "AgilitySystem.h"

#include "EnergyComponent.h"

int constexpr AGILITY_TO_REGEN_RATIO = 1;

[[nodiscard]]
EnergyComponent const& updateEnergy(
    EnergyComponent& energy,
    int agility
)
{
    energy.regenRate = energy.baseRegen + agility * AGILITY_TO_REGEN_RATIO;

    return energy;
}

namespace AgilitySystem
{
    int increaseAgility(
        int agility,
        EnergyComponent& energyIO,
        int value
    )
    {
        agility += value;

        energyIO = updateEnergy(
            energyIO,
            agility
        );

        return agility;
    }
}
