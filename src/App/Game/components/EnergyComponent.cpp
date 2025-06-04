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
}

