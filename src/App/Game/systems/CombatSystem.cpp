#include "CombatSystem.h"

#include "DamageComponent.h"
#include "HealthComponent.h"

namespace CombatSystem
{
    void performAttack(
        DamageComponent const& attackerDamage,
        size_t attackerId,
        HealthComponent& defenderHealthIO
    )
    {
        defenderHealthIO = HealthModule::damage(
            defenderHealthIO,
            DamageModule::damageRNG( attackerDamage ),
            attackerId
        );

        return;
    }
}
