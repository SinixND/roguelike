#ifndef IG20250312002634
#define IG20250312002634

struct AttributesComponent;
struct EnergyComponent;
struct HealthComponent;

namespace AttributeSystem
{
    [[nodiscard]]
    HealthComponent const& updateFromVitality(
        HealthComponent& healthIO,
        int vitality,
        int attributePointsTotal
    );

    [[nodiscard]]
    EnergyComponent const& updateFromAgility(
        EnergyComponent& energyIO,
        int agility,
        int attributePointsTotal
    );

    void updateStats(
        HealthComponent& healthIO,
        EnergyComponent& energyIO,
        AttributesComponent const& attributes
    );
}

#endif
