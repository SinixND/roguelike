#include "Energy.h"

void Energy::setRegenRate(int regenRate) { regenRate_ = regenRate; }

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
        isIdle_ = false;
        return false;
    }

    return true;
}

bool Energy::regenerate()
{
    if (currentEnergy_ >= maxEnergy_)
    {
        isIdle_ = true;
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

int Energy::isIdle() const
{
    return isIdle_;
}
