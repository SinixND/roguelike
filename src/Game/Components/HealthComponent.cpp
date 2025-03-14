#include "HealthComponent.h"

// #define DEBUG_HEALTH

#include "EventDispatcher.h"
#include <cassert>

#if defined( DEBUG ) && defined( DEBUG_HEALTH )
#include "Debugger.h"
#include <string>
#endif

namespace HealthModule
{
    bool damage(
        HealthComponent& healthIO,
        int value
    )
    {
#if defined( DEBUG ) && defined( DEBUG_HEALTH )
        snx::Debugger::cliLog( "Remove " + std::to_string( value ) + " from " + std::to_string( healthIO.currentHealth ) + " health\n" );
#endif
        if ( value )
        {
            healthIO.current -= value;
        }
        else
        {
            healthIO.current = 0;
        }

        snx::EventDispatcher::notify( EventId::INTERRUPT_MOVEMENT );

        if ( healthIO.current <= 0 )
        {
            return true;
        }

        return false;
    }

    HealthComponent const& heal(
        HealthComponent& health,
        int value
    )
    {
        assert( value >= 0 && "Can't heal by negative value" );

        health.current += value;

        if ( health.current > health.maximum )
        {
            health.current = health.maximum;
        }

        return health;
    }

    HealthComponent const& regenerate( HealthComponent& health )
    {
#if defined( DEBUG ) && defined( DEBUG_HEALTH )
        snx::Debugger::cliLog( "Gain " + std::to_string( health.regenRate ) + " to " + std::to_string( health.currentHealth ) + " health\n" );
#endif

        health = HealthModule::heal(
            health,
            health.regenRate
        );

        return health;
    }
}
