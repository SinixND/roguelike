#include "DamageComponent.h"
// #define DEBUG_DAMAGE

#include "RNG.h"

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
#include "Debugger.h"
#include <string>
#endif

int DamageComponent::damage() const
{
    int damage{damage_};

    // Handle critical hit
    if (snx::RNG::random(0, 100) < criticalHitChance_)
    {
#if defined(DEBUG) && defined(DEBUG_DAMAGE)
        snx::debug::cliLog("Critical hit!\n");
#endif
        damage += damage * criticalHitDamageFactor();
    }

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
    snx::debug::cliLog(std::to_string(damage) + " damage\n");
#endif
    return damage;
}

float DamageComponent::damageAverage() const
{
    return damage_ + (damage_ * criticalHitChanceFactor() * criticalHitDamageFactor());
}

int DamageComponent::criticalHitChancePercent() const
{
    return criticalHitChance_;
}

float DamageComponent::criticalHitChanceFactor() const
{
    return criticalHitChance_ / 100.0f;
}

int DamageComponent::criticalHitDamagePercent() const
{
    return criticalHitDamage_;
}

float DamageComponent::criticalHitDamageFactor() const
{
    return criticalHitDamage_ / 100.0f;
}
