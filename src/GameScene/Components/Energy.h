#ifndef IG20240601002118
#define IG20240601002118

class Energy
{
    int maxEnergy_{};
    int currentEnergy_{};
    int regenRate_{};

public:
    void setMaxEnergy(int maxEnergy) { maxEnergy_ = maxEnergy; }
    void setRegenRate(int rate) { regenRate_ = rate; }

    // Consumes all energy if no parameter provided and returns false if current energy level is not sufficient
    bool consume(int energy = -1);

    // Returns true if energy full
    bool regenerate();
    int isFull() const { return currentEnergy_ == maxEnergy_; }
};

#endif