#ifndef IG20240601002118
#define IG20240601002118

class Energy
{
    int maxEnergy_{10};
    int currentEnergy_{maxEnergy_};
    int regenRate_{1};
    bool isReady_{true};

public:
    Energy() = default;

    explicit Energy(int regenRate)
        : regenRate_(regenRate)
    {
    }

    void setRegenRate(int regenRate);

    // Consumes all energy if no parameter provided and returns if there is energy left
    bool consume(int energy = -1);

    // Returns true if energy full
    bool regenerate();
    int isReady() const;
};

#endif
