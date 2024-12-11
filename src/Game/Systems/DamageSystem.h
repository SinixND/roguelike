#ifndef IG20241029004032
#define IG20241029004032

struct DamageComponent;
class HealthComponent;

namespace DamageSystem
{
    //* Returns true if defender health became 0
    void attack(
        DamageComponent const& attackerDamage,
        HealthComponent& defenderHealth);
}

#endif
