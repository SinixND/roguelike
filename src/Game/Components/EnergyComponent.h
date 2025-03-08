#ifndef IG20240601002118
#define IG20240601002118

#include <cassert>

struct EnergyComponent
{
    enum class State
    {
        READY,
        NOT_READY,
    };

    int regenRate{};
    int maxEnergy{};
    int currentEnergy{ maxEnergy };

    State state{ State::READY };
};

namespace EnergyModule
{
    //* Consumes all energy remaining; Returns if consumption was successful
    bool consume( EnergyComponent& energyIO );

    //* Consumes energy; Returns if consumption was successful
    bool consume(
        EnergyComponent& energyIO,
        int value
    );

    //* Returns true if energy is full
    bool regenerate( EnergyComponent& energyIO );
}
#endif
