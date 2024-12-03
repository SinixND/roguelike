#ifndef IG20241028234910
#define IG20241028234910

class HealthComponent
{
    int currentHealth_{};

public:
    int maxHealth{};
    int regenRate{};

public:
    HealthComponent() = default;

    explicit HealthComponent(int maxHealth, int regenRate = 0)
        : currentHealth_(maxHealth)
        , maxHealth(maxHealth)
        , regenRate(regenRate)
    {
    }

    // Returns true if health is <= 0
    // Reduces health to 0 with no argument
    bool damage(int health = 0);

    // Fully heals with no argument
    void heal(int health = 0);

    int currentHealth() const;
};

void regenerate(HealthComponent* healthComponent);

#endif
