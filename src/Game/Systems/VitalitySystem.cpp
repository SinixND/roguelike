#include "VitalitySystem.h"

#include "HealthComponent.h"

int constexpr VITALITY_TO_HEALTH_RATIO = 2;

[[nodiscard]]
HealthComponent const& updateHealth(
    HealthComponent& health,
    int vitality
)
{
    health.maximum = health.base + vitality * VITALITY_TO_HEALTH_RATIO;
    health.current = health.maximum;

    return health;
}

namespace VitalitySystem
{
    int increaseVitality(
        int vitality,
        HealthComponent& healthIO,
        int value
    )
    {
        vitality += value;

        healthIO = updateHealth(
            healthIO,
            vitality
        );

        return vitality;
    }
}
