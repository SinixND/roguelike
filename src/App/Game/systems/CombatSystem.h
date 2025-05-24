#ifndef IG20250320163415
#define IG20250320163415

struct DamageComponent;
struct HealthComponent;

namespace CombatSystem
{
    void performAttack(
        DamageComponent const& attackerDamage,
        HealthComponent& defenderHealthIO
    );
}

#endif
