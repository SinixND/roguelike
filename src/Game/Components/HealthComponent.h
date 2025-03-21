#ifndef IG20241028234910
#define IG20241028234910

struct HealthComponent
{
    int base{ 10 };
    int regenRate{ 0 };
    int maximum{ base };
    int current{ maximum };
};

namespace HealthModule
{
    //* Reduces health to 0 with no argument
    [[nodiscard]]
    HealthComponent const& damage(
        HealthComponent& health,
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
