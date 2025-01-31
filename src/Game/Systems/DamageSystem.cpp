#include "DamageSystem.h"

#include "DamageComponent.h"
#include "HealthComponent.h"

namespace DamageSystem
{
    void attack(
        DamageComponent const& attackerDamage,
        HealthComponent& defenderHealth
    )
    {
        HealthModule::damage(
            &defenderHealth,
            DamageModule::damageRNG( attackerDamage )
        );
    }
}
