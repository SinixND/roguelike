#include "HealthComponent.h"
#include "PublisherStatic.h"
#define DEBUG_HEALTH

#if defined( DEBUG ) && defined( DEBUG_HEALTH )
#include "Debugger.h"
#include <string>
#endif

bool HealthModule::damage(
    HealthComponent& health,
    int value
)
{
#if defined( DEBUG ) && defined( DEBUG_HEALTH )
    snx::debug::cliLog( "Remove " + std::to_string( value ) + " from " + std::to_string( health.currentHealth ) + " health\n" );
#endif
    if ( value )
    {
        health.currentHealth -= value;
    }
    else
    {
        health.currentHealth = 0;
    }

    snx::PublisherStatic::publish( EventId::INTERRUPT_MOVEMENT );

    if ( health.currentHealth <= 0 )
    {
        return true;
    }

    return false;
}

void HealthModule::heal(
    HealthComponent& health,
    int value
)
{
    if ( value )
    {
        health.currentHealth += value;
    }
    else
    {
        health.currentHealth = health.maxHealth;
    }

    if ( health.currentHealth > health.maxHealth )
    {
        health.currentHealth = health.maxHealth;
    }
}

void HealthModule::regenerate( HealthComponent& health )
{
#if defined( DEBUG ) && defined( DEBUG_HEALTH )
    snx::debug::cliLog( "Gain " + std::to_string( health.regenRate ) + " to " + std::to_string( health.currentHealth ) + " health\n" );
#endif

    HealthModule::heal(
        health,
        health.regenRate
    );
}
