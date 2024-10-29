#include "DamageSystem.h"

#include "DamageComponent.h"
#include "HealthComponent.h"

bool DamageSystem::attack(
    DamageComponent const& attackerDamage,
    HealthComponent& defenderHealth)
{
    return defenderHealth.damage(attackerDamage.damage());
}