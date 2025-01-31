#ifndef IG20240601002118
#define IG20240601002118

struct EnergyComponent
{
    enum class State
    {
        READY,
        NOT_READY,
    };

    int regenRate{ 1 };
    int maxEnergy{ 10 };
    int currentEnergy{ maxEnergy };

    State state{ State::READY };
};

namespace EnergyModule
{
    //* Consumes all energy remaining; Returns if consumption was successful
    bool consume( EnergyComponent* energy );

    //* Consumes energy; Returns if consumption was successful
    bool consume(
        EnergyComponent* energy,
        int value
    );

    //* Returns true if energy was regenerated
    bool regenerate( EnergyComponent* energy );
}
#endif
