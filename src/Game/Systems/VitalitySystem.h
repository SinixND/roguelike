#ifndef IG20250309180301
#define IG20250309180301

struct HealthComponent;

namespace VitalitySystem
{
    [[nodiscard]]
    int increaseVitality(
        int vitality,
        HealthComponent& healthIO,
        int value = 1
    );
}

#endif
