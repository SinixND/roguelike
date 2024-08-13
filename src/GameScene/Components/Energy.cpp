#include "Energy.h"

#include "Debugger.h"
#include "DeveloperMode.h"
#include <string>

bool Energy::consume(int energy)
{
    if (DeveloperMode::isActive())
    {
        snx::debug("Consume " + std::to_string(energy) + " energy.\n");
    }

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
    if (DeveloperMode::isActive())
    {
        snx::debug("Regenerate " + std::to_string(regenRate_) + " energy.\n");
    }

    if (currentEnergy_ > maxEnergy_)
    {
        // Ensure energy does not exceed maxEnergy
        currentEnergy_ = maxEnergy_;
    }
    return false;
}