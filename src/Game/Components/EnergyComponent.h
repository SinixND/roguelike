#ifndef IG20240601002118
#define IG20240601002118

enum class EnergyState
{
    READY,
    NOT_READY,
};

struct EnergyComponent
{
    int regenRate{ 1 };
    int maxEnergy{ 10 };
    int currentEnergy{ maxEnergy };

    EnergyState state{ EnergyState::READY };
};

namespace EnergyModule
{
    //* Consumes all energy remaining if no parameter provided; returns if consumption was successful
    bool consume( EnergyComponent& energy );

    bool consume(
        EnergyComponent& energy,
        int value
    );

    //* Returns true if regeneration was successful
    bool regenerate( EnergyComponent& energy );
}
#endif
