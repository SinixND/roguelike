#ifndef IG20240601002118
#define IG20240601002118

#include <cassert>

/// Unit can perform action(s) if energy is full (-> READY)
/// Unit can perform action(s) until energy is < 0 (-> NOT_READY)
/// Energy can be < 0
/// Default action cost is 10
struct EnergyComponent
{
    enum class State
    {
        READY,
        NOT_READY,
    };

    int regenRate{ 1 };
    int maxEnergy{ 5 };
    int currentEnergy{ maxEnergy };

    State state{ State::READY };
};

namespace EnergyModule
{
    //* Consumes all energy remaining; Returns if consumption was successful
    bool exhaust( EnergyComponent& energyIO );

    //* Consumes energy; Returns if consumption was successful
    bool consume(
        EnergyComponent& energyIO,
        int value
    );

    //* Returns true if energy is full
    bool regenerate( EnergyComponent& energyIO );
}

#endif

