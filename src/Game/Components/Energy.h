#ifndef IG20240601002118
#define IG20240601002118

class Energy
{
    int maxEnergy_{10};
    int currentEnergy_{maxEnergy_};
    int regenRate_{1};
    bool isIdle_{true};

public:
    Energy() = default;

    Energy(int agility)
        : regenRate_(agility)
    {
    }

    void setMaxEnergy(int maxEnergy);
    void setRegenRate(int regenRate);
    void setMax();

    // Consumes all energy if no parameter provided and returns if there is energy left
    bool consume(int energy = -1);

    // Returns true if energy full
    bool regenerate();
    int isIdle() const;
};

#endif
