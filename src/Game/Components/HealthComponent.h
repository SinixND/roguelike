#ifndef IG20241028234910
#define IG20241028234910

class HealthComponent
{
    int maxHealth_{};
    int currentHealth_{};
    int regenRate_{};

public:
    HealthComponent() = default;

    explicit HealthComponent(int maxHealth, int regenRate = 0)
        : maxHealth_(maxHealth)
        , currentHealth_(maxHealth)
        , regenRate_(regenRate)
    {
    }

    int maxHealth() const;

    void increaseMaxHealth(int health);

    void reduceMaxHealth(int health);

    int currentHealth() const;

    // Returns true if health is <= 0
    // Reduces health to 0 with no argument
    bool damage(int health = 0);

    // Fully heals with no argument
    void heal(int health = 0);

    void regenerate();
};

#endif