#include "HealthComponent.h"
#define DEBUG_HEALTH

#if defined(DEBUG) && defined(DEBUG_HEALTH)
#include "Debugger.h"
#include <string>
#endif

bool damage(
    HealthComponent* healthComponent,
    int health)
{
#if defined(DEBUG) && defined(DEBUG_HEALTH)
    snx::debug::cliLog("Remove " + std::to_string(health) + " from " + std::to_string(healthComponent->currentHealth) + " health\n");
#endif
    if (health)
    {
        healthComponent->currentHealth -= health;
    }
    else
    {
        healthComponent->currentHealth = 0;
    }

    if (healthComponent->currentHealth <= 0)
    {
        return true;
    }

    return false;
}

void heal(
    HealthComponent* healthComponent,
    int health)
{
    if (health)
    {
        healthComponent->currentHealth += health;
    }
    else
    {
        healthComponent->currentHealth = healthComponent->maxHealth;
    }

    if (healthComponent->currentHealth > healthComponent->maxHealth)
    {
        healthComponent->currentHealth = healthComponent->maxHealth;
    }
}

void regenerate(HealthComponent* healthComponent)
{
#if defined(DEBUG) && defined(DEBUG_HEALTH)
    snx::debug::cliLog("Gain " + std::to_string(healthComponent->regenRate) + " to " + std::to_string(healthComponent->currentHealth) + " health\n");
#endif
    heal(
        healthComponent,
        healthComponent->regenRate);
}
