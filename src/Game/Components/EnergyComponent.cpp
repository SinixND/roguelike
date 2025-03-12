#include "EnergyComponent.h"

#include <cassert>

// #define DEBUG_ENERGY

#if defined( DEBUG ) && defined( DEBUG_ENERGY )
#include "Debugger.h"
#endif

namespace EnergyModule
{
    bool exhaust( EnergyComponent& energyIO )
    {
        //* Can't consume if not ready
        if ( !isReady( energyIO ) )
        {
            return false;
        }

//* Consume energy
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::debug::cliLog( "EnergyModule::exhaust()\n" );
#endif
        energyIO.current = 0;

        return true;
    }

    bool regenerate( EnergyComponent& energyIO )
    {
        //* If already full
        if ( energyIO.current >= ENERGY_MAX )
        {
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
            snx::debug::cliPrint( "Energy is full.\n" );
#endif
            return true;
        }

        //* Regen energy until full
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::debug::cliPrint( "Regen ", energyIO.currentEnergy, "+", energyIO.regenRate, "\n" );
#endif
        energyIO.current += energyIO.regenRate;

        if ( energyIO.current < ENERGY_MAX )
        {
            return false;
        }

        return true;
    }

    bool isReady( EnergyComponent const& energy )
    {
        return !( energy.current < ENERGY_MAX );
    }
}

