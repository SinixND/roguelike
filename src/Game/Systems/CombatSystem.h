#ifndef IG20250320163415
#define IG20250320163415

struct EnergyComponent;
struct DamageComponent;
struct HealthComponent;

namespace CombatSystem
{
    void performAttack(
        EnergyComponent& attackerEnergyIO,
        DamageComponent const& attackerDamage,
        HealthComponent& defenderHealthIO
    );
}

#endif
