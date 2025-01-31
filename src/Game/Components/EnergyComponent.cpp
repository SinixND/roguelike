#include "EnergyComponent.h"
#include <cassert>

namespace EnergyModule
{
    bool consume( EnergyComponent& energyIO )
    {
        if ( energyIO.currentEnergy <= 0 )
        {
            return false;
        }

        //* Consume all available energy
        energyIO.currentEnergy = 0;

        energyIO.state = EnergyComponent::State::NOT_READY;

        return true;
    }

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
        energyIO.currentEnergy -= value;

        if ( energyIO.currentEnergy <= 0 )
        {
            energyIO.state = EnergyComponent::State::NOT_READY;
        }

        return true;
    }

    bool regenerate( EnergyComponent& energyIO )
    {
        //* If already full
        if ( energyIO.currentEnergy >= energyIO.maxEnergy )
        {
            return false;
        }

        //* Regen energy until full
        energyIO.currentEnergy += energyIO.regenRate;

        if ( energyIO.currentEnergy < energyIO.maxEnergy )
        {
            return true;
        }

        //* Energy full: Ensure energy does not exceed maxEnergy
        energyIO.currentEnergy = energyIO.maxEnergy;
        energyIO.state = EnergyComponent::State::READY;

        return true;
    }
}

