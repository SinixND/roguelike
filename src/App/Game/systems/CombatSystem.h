#ifndef IG20250320163415
#define IG20250320163415

#include <cstddef>

struct DamageComponent;
struct HealthComponent;

namespace CombatSystem
{
    void performAttack(
        DamageComponent const& attackerDamage,
        size_t attackerId,
        HealthComponent& defenderHealthIO
    );
}

#endif
