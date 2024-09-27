#include "Energy.h"

void Energy::setMaxEnergy(int maxEnergy) { maxEnergy_ = maxEnergy; }

void Energy::setRegenRate(int agility) { regenRate_ = agility; }

bool Energy::consume(int energy)
{
    if (energy < 0)
    {
        // Consume all available energy
        currentEnergy_ = 0;
    }
    else
    {
        // Consume energy
        currentEnergy_ -= energy;
    }

    if (currentEnergy_ <= 0)
    {
        isExhausted_ = true;
        return false;
    }

    return true;
}

bool Energy::regenerate()
{
    if (currentEnergy_ >= maxEnergy_)
    {
        isExhausted_ = false;
        return true;
    }

    // Regen energy until full
    currentEnergy_ += regenRate_;

    if (currentEnergy_ > maxEnergy_)
    {
        // Ensure energy does not exceed maxEnergy
        currentEnergy_ = maxEnergy_;
        return true;
    }

    return false;
}

int Energy::isExhausted() const
{
    return isExhausted_;
}
