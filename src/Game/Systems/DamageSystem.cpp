#include "DamageSystem.h"
#include "DamageComponent.h"
#include "HealthComponent.h"

void DamageSystem::attack(
    DamageComponent const& attackerDamage,
    HealthComponent& defenderHealth)
{
    defenderHealth.damage(damageRNG(attackerDamage));
}
