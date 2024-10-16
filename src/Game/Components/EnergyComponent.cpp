#include "EnergyComponent.h"

bool EnergyComponent::consume(int energy)
{
    if (energy < 0)
    {
        //* Consume all available energy
        currentEnergy_ = 0;
    }
    else
    {
        //* Consume energy
        currentEnergy_ -= energy;
    }

    if (currentEnergy_ <= 0)
    {
        isReady_ = false;
        return false;
    }

    return true;
}

bool EnergyComponent::regenerate()
{
    if (currentEnergy_ >= maxEnergy_)
    {
        isReady_ = true;
        return true;
    }

    //* Regen energy until full
    currentEnergy_ += regenRate;

    if (currentEnergy_ > maxEnergy_)
    {
        //* Ensure energy does not exceed maxEnergy
        currentEnergy_ = maxEnergy_;
        return true;
    }

    return false;
}

int EnergyComponent::isReady() const
{
    return isReady_;
}
