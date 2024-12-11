#include "HealthComponent.h"
#define DEBUG_HEALTH

#if defined(DEBUG) && defined(DEBUG_HEALTH)
#include "Debugger.h"
#include <string>
#endif

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
        currentHealth_ = maxHealth;
    }

    if (currentHealth_ > maxHealth)
    {
        currentHealth_ = maxHealth;
    }
}

void regenerate(HealthComponent& healthComponent)
{
#if defined(DEBUG) && defined(DEBUG_HEALTH)
    snx::debug::cliLog("Gain " + std::to_string(healthComponent.regenRate) + " to " + std::to_string(healthComponent.currentHealth()) + " health\n");
#endif

    healthComponent.heal(healthComponent.regenRate);
}
