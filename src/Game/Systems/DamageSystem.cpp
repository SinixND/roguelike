#include "DamageSystem.h"
#include "DamageComponent.h"
#include "HealthComponent.h"

void DamageSystem::attack(
    DamageComponent const& attackerDamage,
    HealthComponent& defenderHealth
)
{
    HealthModule::damage(
        defenderHealth,
        DamageModule::damageRNG( attackerDamage )
    );
}
