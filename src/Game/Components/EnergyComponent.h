#ifndef IG20240601002118
#define IG20240601002118

class EnergyComponent
{
    int currentEnergy_{};
    bool isReady_{true};

public:
    int maxEnergy_{10};
    int regenRate{1};

public:
    EnergyComponent() = default;

    EnergyComponent(
        int maxEnergy,
        int regenRate)
        : currentEnergy_(maxEnergy)
        , maxEnergy_(maxEnergy)
        , regenRate(regenRate)
    {
    }

    //* Consumes all energy remaining if no parameter provided; returns if consumption was successful
    bool consume();
    bool consume(int energy);

    //* Returns true if regeneration was successful
    bool regenerate();

    bool isReady() const;
};

#endif
