#include "EnergyComponent.h"

bool EnergyModule::consume( EnergyComponent& energy )
{
    if ( energy.currentEnergy <= 0 )
    {
        return false;
    }

    //* Consume all available energy
    energy.currentEnergy = 0;

    energy.state = EnergyState::NOT_READY;

    return true;
}

bool EnergyModule::consume(
    EnergyComponent& energy,
    int value
)
{
    if ( energy.currentEnergy <= 0 )
    {
        return false;
    }

    //* Consume energy
    energy.currentEnergy -= value;

    if ( energy.currentEnergy <= 0 )
    {
        energy.state = EnergyState::NOT_READY;
    }

    return true;
}

bool EnergyModule::regenerate( EnergyComponent& energy )
{
    if ( energy.currentEnergy >= energy.maxEnergy )
    {
        return false;
    }

    //* Regen energy until full
    energy.currentEnergy += energy.regenRate;

    if ( energy.currentEnergy < energy.maxEnergy )
    {
        return true;
    }

    //* Ensure energy does not exceed maxEnergy
    energy.currentEnergy = energy.maxEnergy;
    energy.state = EnergyState::READY;

    return true;
}

