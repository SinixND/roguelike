#ifndef IG20240601002118
#define IG20240601002118

#include <cassert>

/// Equals default action cost
float constexpr ENERGY_REGEN_MAX{ 60.0f };

/// Maximum speed difference between slowest and fastest
float constexpr SPEED_RATIO_MAX{ 4.0f };

/// Helper values
float const ENERGY_REGEN_MIN{ ENERGY_REGEN_MAX / SPEED_RATIO_MAX };

/// Unit can perform action(s) if energy is full (-> READY)
struct EnergyComponent
{
    float regenRate{ 0 };
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
}

#endif

