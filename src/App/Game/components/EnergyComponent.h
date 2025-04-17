#ifndef IG20240601002118
#define IG20240601002118

#include "EnergyData.h"
#include <cassert>

/// Unit can perform action(s) if energy is full (-> READY)
struct EnergyComponent
{
    float regenRate{ EnergyData::ENERGY_REGEN_AVG };
    float current{ 0 };
};

namespace EnergyModule
{
    /// Consume proportional energy
    [[nodiscard]]
    EnergyComponent const& consume(
        EnergyComponent& energy,
        float multiplier
    );

    /// Consume default cost
    [[nodiscard]]
    EnergyComponent const& exhaust( EnergyComponent& energy );

    /// Returns true if energy is full
    [[nodiscard]]
    EnergyComponent const& regenerate( EnergyComponent& energy );
}

#endif

