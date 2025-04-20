#ifndef IG20250420195428
#define IG20250420195428

#include "DenseMap.h"

struct EnergyComponent;
struct FlagComponent;

namespace EnergySystem
{
    /// Regen energy until a unit becomes ready to act
    void udpate(
        EnergyComponent& heroEnergyIO,
        bool& heroIsReadyIO,
        snx::DenseMap<size_t, EnergyComponent>& enemyEnergiesIO,
        snx::DenseMap<size_t, FlagComponent>& enemyIsReadiesIO
    );
}

#endif
