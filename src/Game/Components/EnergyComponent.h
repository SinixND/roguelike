#ifndef IG20240601002118
#define IG20240601002118

#include <cassert>
#include <cmath>

/// Equals default action cost
float constexpr ENERGY_REGEN_MAX{ 36.0f };

/// Speed can vary between multiplier and 1/multiplier
float constexpr SPEED_MULTIPLIER_MAX{ 2.0f };

float const ENERGY_REGEN_AVG{ ENERGY_REGEN_MAX / SPEED_MULTIPLIER_MAX };
float const ENERGY_REGEN_MIN{ ENERGY_REGEN_MAX / std::pow( SPEED_MULTIPLIER_MAX, 2.0f ) };

/// Unit can perform action(s) if energy is full (-> READY)
struct EnergyComponent
{
    float regenRate{ ENERGY_REGEN_AVG };
    float current{ 0 };
    bool isReady{ true };
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
    bool regenerate( EnergyComponent& energyIO );
}

#endif

