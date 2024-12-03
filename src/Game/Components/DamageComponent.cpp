#include "DamageComponent.h"

// #define DEBUG_DAMAGE

#include "Logger.h"
#include "RNG.h"
#include <raylib.h>

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
#include "Debugger.h"
#include <string>
#endif

int DamageComponent::baseDamage() const
{
    return baseDamage_;
}

int DamageComponent::damage() const
{
    int damage{baseDamage_};

    // Handle critical hit
    bool isCrit_ = snx::RNG::random(0, 100) < criticalHitChance_;

    if (isCrit_)
    {
#if defined(DEBUG) && defined(DEBUG_DAMAGE)
        snx::debug::cliLog("Critical hit!\n");
#endif
        damage += damage * criticalHitDamageFactor();
    }

#if defined(DEBUG) && defined(DEBUG_DAMAGE)
    snx::debug::cliLog(std::to_string(damage) + " damage\n");
#endif
    snx::Logger::logAppend(TextFormat("%i damage", damage));

    if (isCrit_)
    {
        snx::Logger::logAppend(" (Crit!)");
    }

    return damage;
}

float DamageComponent::damageAverage() const
{
    return baseDamage_ + (baseDamage_ * criticalHitChanceFactor() * criticalHitDamageFactor());
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
