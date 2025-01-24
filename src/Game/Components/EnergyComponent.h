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
    //* Consumes all energy remaining; Returns if consumption was successful
    bool consume( EnergyComponent& energy );

    //* Consumes energy; Returns if consumption was successful
    bool consume(
        EnergyComponent& energy,
        int value
    );

    //* Returns true if energy was regenerated
    bool regenerate( EnergyComponent& energy );
}
#endif
