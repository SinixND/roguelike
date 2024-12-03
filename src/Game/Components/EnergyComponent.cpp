#include "EnergyComponent.h"

bool EnergyComponent::consume()
{
    if (currentEnergy_ <= 0)
    {
        return false;
    }

    //* Consume all available energy
    currentEnergy_ = 0;

    isReady_ = false;

    return true;
}

bool EnergyComponent::consume(int energy)
{
    if (currentEnergy_ <= 0)
    {
        return false;
    }

    //* Consume energy
    currentEnergy_ -= energy;

    if (currentEnergy_ <= 0)
    {
        isReady_ = false;
    }

    return true;
}

bool EnergyComponent::regenerate()
{
    if (currentEnergy_ >= maxEnergy_)
    {
        return false;
    }

    //* Regen energy until full
    currentEnergy_ += regenRate;

    if (currentEnergy_ < maxEnergy_)
    {
        return true;
    }

    //* Ensure energy does not exceed maxEnergy
    currentEnergy_ = maxEnergy_;
    isReady_ = true;

    return true;
}

bool EnergyComponent::isReady() const
{
    return isReady_;
}
