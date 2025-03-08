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
        if ( energyIO.currentEnergy <= 0 )
        {
            return false;
        }

//* Consume energy
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::debug::cliLog( "EnergyModule::consume()\n" );
#endif
        energyIO.currentEnergy -= value;

        if ( energyIO.currentEnergy <= 0 )
        {
            energyIO.state = EnergyComponent::State::NOT_READY;
        }

        return true;
    }

    bool consume( EnergyComponent& energyIO )
    {
        return consume( energyIO, energyIO.currentEnergy );
    }

    bool regenerate( EnergyComponent& energyIO )
    {
        //* If already full
        if ( energyIO.currentEnergy >= energyIO.maxEnergy )
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
        energyIO.currentEnergy += energyIO.regenRate;

        if ( energyIO.currentEnergy < energyIO.maxEnergy )
        {
            return false;
        }

        //* Energy full: Ensure energy does not exceed maxEnergy
        energyIO.currentEnergy = energyIO.maxEnergy;
        energyIO.state = EnergyComponent::State::READY;

        return true;
    }
}

