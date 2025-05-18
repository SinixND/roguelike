#include "EnergyComponent.h"

#include <cassert>

// #define DEBUG_ENERGY

#if defined( DEBUG ) && defined( DEBUG_ENERGY )
#include "Debugger.h"
#endif

namespace EnergyModule
{
    EnergyComponent const& consume(
        EnergyComponent& energy,
        float multiplier
    )
    {
        //* Can't consume if not ready
        if ( energy.current < 0 )
        {
            return energy;
        }

        //* Consume energy
        energy.current -= ENERGY_REGEN_MAX * multiplier;

        return energy;
    }

    EnergyComponent const& exhaust( EnergyComponent& energy )
    {
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::Debugger::cliLog( "EnergyModule::exhaust()\n" );
#endif
        energy = consume(
            energy,
            1.0f
        );

        return energy;
    }

    //* TODO: CHANGE/REMOVE
    bool regenerate( EnergyComponent& energyIO )
    {
        //* If already full
        if ( !( energyIO.current < 0 ) )
        {
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
            snx::Debugger::cliPrint( "Energy is full.\n" );
#endif
            return true;
        }

        //* Regen energy until full
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::Debugger::cliPrint( "Regen ", energyIO.current, "+", energyIO.regenRate, "\n" );
#endif
        energyIO.current += energyIO.regenRate;

        if ( energyIO.current < 0 )
        {
            return false;
        }

        return true;
    }
}

