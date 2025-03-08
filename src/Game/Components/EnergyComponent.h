#ifndef IG20240601002118
#define IG20240601002118

#include <cassert>

/// Unit can perform action(s) if energy is full (-> READY)
struct EnergyComponent
{
    /// RegenRate = (max + AGI) / weight
    /// Default weight is 4
    int baseRegen{ 4 };
    int regenRate{ 4 };
    int maximum{ 16 };
    int current{ maximum };
};

namespace EnergyModule
{

    //* Consumes energy; Returns if consumption was successful
    bool consume(
        EnergyComponent& energyIO,
        int value
    );

    //* Consumes all energy remaining; Returns if consumption was successful
    bool exhaust( EnergyComponent& energyIO );

    //* Returns true if energy is full
    bool regenerate( EnergyComponent& energyIO );

    bool isReady( EnergyComponent const& energy );
}

#endif

