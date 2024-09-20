#ifndef IG20240601002118
#define IG20240601002118

class Energy
{
    int maxEnergy_{};
    int currentEnergy_{};
    int regenRate_{};

public:
    Energy() = default;

    Energy(int maxEnergy, int regenRate)
        : maxEnergy_(maxEnergy)
        , regenRate_(regenRate)
    {
    }

    void setMaxEnergy(int maxEnergy);
    void setRegenRate(int rate);

    // Consumes all energy if no parameter provided and returns false if current energy level is not sufficient
    bool consume(int energy = -1);

    // Returns true if energy full
    bool regenerate();
    int isFull() const;
};

#endif
