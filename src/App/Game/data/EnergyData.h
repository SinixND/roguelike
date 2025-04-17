#ifndef IG20250417200334
#define IG20250417200334

#include <cmath>

namespace EnergyData
{
    /// Equals default action cost
    float constexpr ENERGY_REGEN_MAX{ 36.0f };

    /// Resulting speed can vary between multiplier_max and 1/multiplier_max
    float constexpr SPEED_MULTIPLIER_MAX{ 2.0f };

    float const ENERGY_REGEN_AVG{ ENERGY_REGEN_MAX / SPEED_MULTIPLIER_MAX };
    float const ENERGY_REGEN_MIN{ ENERGY_REGEN_MAX / std::pow( SPEED_MULTIPLIER_MAX, 2.0f ) };
}

#endif

