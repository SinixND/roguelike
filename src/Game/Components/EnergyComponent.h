#ifndef IG20240601002118
#define IG20240601002118

class EnergyComponent
{
    int maxEnergy_{10};
    int currentEnergy_{maxEnergy_};
    bool isReady_{true};

public:
    int regenRate{1};

public:
    EnergyComponent() = default;

    explicit EnergyComponent(int regenRate)
        : regenRate(regenRate)
    {
    }

    //* Consumes all energy if no parameter provided and returns if there is energy left
    bool consume(int energy = -1);

    //* Returns true if energy full
    bool regenerate();

public:
    int isReady() const;
};

#endif
