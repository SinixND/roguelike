#include "Energy.h"
#include "Logger.h"

bool Energy::consume(int energy)
{
    if (!energy)
    {
        // Consume all energy
        energy_ = 0;
        return true;
    }
    else if (energy < energy_)
    {
        // Partial energy consumption valid
        energy_ -= energy;
        return true;
    }

    // Not enough energy
    return false;
}

bool Energy::regenerate()
{
    if (energy_ >= maxEnergy_)
    {
        return true;
    }

    // Regen energy until full
    energy_ += regenRate_;

    if (energy_ > maxEnergy_)
    {
        // Ensure energy does not exceed maxEnergy
        energy_ = maxEnergy_;
    }
    return false;
}