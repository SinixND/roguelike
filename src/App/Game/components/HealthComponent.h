#ifndef IG20241028234910
#define IG20241028234910

#include <cstddef>

struct HealthComponent
{
    int base{ 10 };
    int regenRate{ 0 };
    int maximum{ base };
    int current{ maximum };
    /// Hero == 1
    size_t lastDamageSourceId{ 0 };
};

namespace HealthModule
{
    //* Reduces health to 0 with no argument
    [[nodiscard]]
    HealthComponent const& damage(
        HealthComponent& health,
        int value,
        size_t damageSourceId
    );

    //* Fully heals with no argument
    [[nodiscard]]
    HealthComponent const& heal(
        HealthComponent& health,
        int value
    );

    [[nodiscard]]
    HealthComponent const& regenerate( HealthComponent& health );
}

#endif
