#include "DamageComponent.h"

// #define DEBUG_DAMAGE

#include "Logger.h"
#include "RNG.h"
#include <raylib.h>

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
#include "Debugger.h"
#include <string>
#endif

int calculateDamage(DamageComponent const& damageComponent)
{
    int damage{damageComponent.baseDamage};

    // Handle critical hit
    damage += damage * (damageComponent.criticalHitDamage * 100.0f) * static_cast<int>(snx::RNG::random(0, 100) < damageComponent.criticalHitChance);

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
    snx::debug::cliLog(std::to_string(damage) + " damage\n");
#endif
    snx::Logger::logAppend(TextFormat("%i damage", damage));

    return damage;
}

float damageAverage(DamageComponent const& damageComponent)
{
    return damageComponent.baseDamage + (damageComponent.baseDamage * (damageComponent.criticalHitChance / 100.0f) * (damageComponent.criticalHitDamage / 100.0f));
}

