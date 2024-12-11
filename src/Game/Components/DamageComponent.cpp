#include "DamageComponent.h"

// #define DEBUG_DAMAGE

#include "Logger.h"
#include "RNG.h"
#include <raylib.h>

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
#include "Debugger.h"
#include <string>
#endif

int damageRNG(DamageComponent const& damageComponent)
{
    int damage = damageComponent.baseDamage + (damageComponent.baseDamage * (snx::RNG::random(.0f, 100.0f) < damageComponent.critChanceFactor) * damageComponent.critDamageFactor);

    snx::Logger::logAppend(TextFormat("%i damage", damage));

    return damage;
}

int damageAverage(DamageComponent const& damageComponent)
{
    return damageComponent.baseDamage + (damageComponent.baseDamage * damageComponent.critChanceFactor * damageComponent.critDamageFactor);
}
