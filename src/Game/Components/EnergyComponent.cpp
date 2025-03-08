#include "EnergyComponent.h"

#include <cassert>

// #define DEBUG_ENERGY

#if defined( DEBUG )
#include "Debugger.h"
#endif

namespace EnergyModule
{
    bool consume(
        EnergyComponent& energyIO,
        int value
    )
    {
        //* Can't consume if not ready
        if ( !isReady( energyIO ) )
        {
            return false;
        }

//* Consume energy
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::debug::cliLog( "EnergyModule::consume()\n" );
#endif
        energyIO.current -= value;

        return true;
    }

    bool exhaust( EnergyComponent& energyIO )
    {
        return consume( energyIO, energyIO.maximum );
    }

    bool regenerate( EnergyComponent& energyIO )
    {
        //* If already full
        if ( energyIO.current >= energyIO.maximum )
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

        if ( energyIO.current < energyIO.maximum )
        {
            return false;
        }

        return true;
    }

    bool isReady( EnergyComponent const& energy )
    {
        return !( energy.current < energy.maximum );
    }
}

