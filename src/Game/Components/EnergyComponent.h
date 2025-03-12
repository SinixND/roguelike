#ifndef IG20240601002118
#define IG20240601002118

#include <cassert>

/// Max energy is determined by maximum movement/action speed increase
/// factor of sqrt(ENERGY_MAX)
int constexpr ENERGY_MAX{ 16 }; /// Equates to a max of 4x speed increase

/// Unit can perform action(s) if energy is full (-> READY)
struct EnergyComponent
{
    // int baseRegen{ 4 };
    float regenRate{ 4.0f };
    // int maximum{ 16 };
    float current{ ENERGY_MAX };
};

namespace EnergyModule
{
    //* Consumes all energy remaining; Returns if consumption was successful
    bool exhaust( EnergyComponent& energyIO );

    //* Returns true if energy is full
    bool regenerate( EnergyComponent& energyIO );

    bool isReady( EnergyComponent const& energy );
}

#endif

