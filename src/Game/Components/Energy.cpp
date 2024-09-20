#include "Energy.h"

void Energy::setMaxEnergy(int maxEnergy) { maxEnergy_ = maxEnergy; }

void Energy::setRegenRate(int rate) { regenRate_ = rate; }

bool Energy::consume(int energy)
{
    if (energy < 0)
    {
        // Consume all energy
        currentEnergy_ = 0;
        return true;
    }

    else if (energy < currentEnergy_)
    {
        // Partial energy consumption valid
        currentEnergy_ -= energy;
        return true;
    }

    // Not enough energy
    return false;
}

bool Energy::regenerate()
{
    if (currentEnergy_ >= maxEnergy_)
    {
        return true;
    }

    // Regen energy until full
    currentEnergy_ += regenRate_;

    if (currentEnergy_ > maxEnergy_)
    {
        // Ensure energy does not exceed maxEnergy
        currentEnergy_ = maxEnergy_;
    }

    return false;
}

int Energy::isFull() const
{
    return !(currentEnergy_ < maxEnergy_);
}