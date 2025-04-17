#include "EnergyComponent.h"
#include "EnergyData.h"

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
        energy.current -= EnergyData::ENERGY_REGEN_MAX * multiplier;

        return energy;
    }

    EnergyComponent const& exhaust( EnergyComponent& energy )
    {
        //* Can't consume if not ready
        if ( energy.current < 0 )
        {
            return energy;
        }

#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::Debugger::cliLog( "EnergyModule::exhaust()\n" );
#endif
        energy = consume(
            energy,
            1.0f
        );

        return energy;
    }

    EnergyComponent const& regenerate( EnergyComponent& energy )
    {
        //* If not already full
        if ( energy.current < 0 )
        {
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
            snx::Debugger::cliPrint( "Regen ", energyIO.current, "+", energyIO.regenRate, "\n" );
#endif

            //* Regen energy until full
            energy.current += energy.regenRate;
        }

        // #if defined( DEBUG ) && defined( DEBUG_ENERGY )
        //         snx::Debugger::cliPrint( "Energy is full.\n" );
        // #endif

        return energy;
    }
}

