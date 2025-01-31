#ifndef IG20241028234910
#define IG20241028234910

struct HealthComponent
{
    int maxHealth{ 1 };
    int regenRate{ 0 };
    int currentHealth{ maxHealth };
};

namespace HealthModule
{
    //* Returns true if health is <= 0
    //* Reduces health to 0 with no argument
    bool damage(
        HealthComponent* health,
        int value = 0
    );

    //* Fully heals with no argument
    [[nodiscard]]
    HealthComponent const& heal(
        HealthComponent& health,
        int value = 0
    );

    [[nodiscard]]
    HealthComponent const& regenerate( HealthComponent& health );
}

#endif
