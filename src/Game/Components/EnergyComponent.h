#ifndef IG20240601002118
#define IG20240601002118

#include <vector>

struct EnergyComponent
{
    int currentEnergy{10};
    int maxEnergy{currentEnergy};
    int regenRate{1};
    bool isReady{true};
};

bool regenerateAll(std::vector<EnergyComponent>* energies);

//* Consumes all energy if no parameter provided and returns if there is energy left
bool consume(
    EnergyComponent* energyComponent,
    int energy = -1);

//* Returns true if energy full
bool regenerate(EnergyComponent* energyComponent);

#endif
