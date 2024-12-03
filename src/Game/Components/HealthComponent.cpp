#include "HealthComponent.h"
#define DEBUG_HEALTH

#if defined(DEBUG) && defined(DEBUG_HEALTH)
#include "Debugger.h"
#include <string>
#endif

int HealthComponent::maxHealth() const
{
    return maxHealth_;
}

void HealthComponent::increaseMaxHealth(int health)
{
    maxHealth_ += health;
}

void HealthComponent::reduceMaxHealth(int health)
{
    maxHealth_ -= health;

    if (maxHealth_ < 1)
    {
        maxHealth_ = 1;
    }
}

int HealthComponent::currentHealth() const
{
    return currentHealth_;
}

bool HealthComponent::damage(int health)
{
#if defined(DEBUG) && defined(DEBUG_HEALTH)
    snx::debug::cliLog("Remove " + std::to_string(health) + " from " + std::to_string(currentHealth_) + " health\n");
#endif
    if (health)
    {
        currentHealth_ -= health;
    }
    else
    {
        currentHealth_ = 0;
    }

    if (currentHealth_ <= 0)
    {
        return true;
    }

    return false;
}

void HealthComponent::heal(int health)
{
    if (health)
    {
        currentHealth_ += health;
    }
    else
    {
        currentHealth_ = maxHealth_;
    }

    if (currentHealth_ > maxHealth_)
    {
        currentHealth_ = maxHealth_;
    }
}

void HealthComponent::regenerate()
{
#if defined(DEBUG) && defined(DEBUG_HEALTH)
    snx::debug::cliLog("Gain " + std::to_string(regenRate_) + " to " + std::to_string(currentHealth_) + " health\n");
#endif
    heal(regenRate_);
}