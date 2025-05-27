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
    HealthComponent const& damage(
        HealthComponent& health,
        int value,
        size_t damageSourceId
    )
    {
#if defined( DEBUG ) && defined( DEBUG_HEALTH )
        snx::Debugger::cliLog( "Remove " + std::to_string( value ) + " from " + std::to_string( healthIO.currentHealth ) + " health\n" );
#endif
        if ( value )
        {
            health.current -= value;
        }
        else
        {
            health.current = 0;
        }

        health.lastDamageSourceId = damageSourceId;

        snx::EventDispatcher::notify( EventId::INTERRUPT_MOVEMENT );

        return health;
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
