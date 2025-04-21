#ifndef IG20250420195428
#define IG20250420195428

#include "DenseMap.h"

struct EnergyComponent;
struct EmptyComponent;

namespace EnergySystem
{
    /// Regen energy until a unit becomes ready to act and mark it as such
    void udpate(
        EnergyComponent& heroEnergyIO,
        bool& heroIsReadyOut,
        snx::DenseMap<size_t, EnergyComponent>& enemyEnergiesIO,
        snx::DenseMap<size_t, EmptyComponent>& enemyIsReadiesOut
    );
}

#endif
