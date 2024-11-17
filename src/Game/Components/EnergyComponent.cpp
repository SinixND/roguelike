#include "EnergyComponent.h"
#include <vector>

bool consume(EnergyComponent* energyComponent, int energy)
{
    if (energy < 0)
    {
        //* Consume all available energy
        energyComponent->currentEnergy = 0;
    }
    else
    {
        //* Consume energy
        energyComponent->currentEnergy -= energy;
    }

    if (energyComponent->currentEnergy <= 0)
    {
        energyComponent->isReady = false;

        return false;
    }

    return true;
}

bool regenerate(EnergyComponent* energyComponent)
{
    if (energyComponent->currentEnergy >= energyComponent->maxEnergy)
    {
        energyComponent->isReady = true;

        return true;
    }

    //* Regen energy until full
    energyComponent->currentEnergy += energyComponent->regenRate;

    if (energyComponent->currentEnergy > energyComponent->maxEnergy)
    {
        //* Ensure energy does not exceed maxEnergy
        energyComponent->currentEnergy = energyComponent->maxEnergy;

        return true;
    }

    return false;
}

bool regenerateAll(std::vector<EnergyComponent>* energies)
{
    bool isReady{false};

    for (EnergyComponent& energy : *energies)
    {
        if (regenerate(&energy))
        {
            isReady = true;
        }
    }

    return isReady;
}
