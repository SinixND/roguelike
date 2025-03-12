#include "AttributeSystem.h"

#include "AttributesComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include <cmath>

namespace AttributeSystem
{
    HealthComponent const& updateFromVitality(
        HealthComponent& healthIO,
        int vitality,
        int attributePointsTotal
    )
    {
        healthIO.maximum = static_cast<int>( healthIO.base * ( 1.0f * vitality / attributePointsTotal ) );
        healthIO.current = healthIO.maximum;

        return healthIO;
    }

    EnergyComponent const& updateFromAgility(
        EnergyComponent& energyIO,
        int agility,
        int attributePointsTotal
    )
    {
        // energyIO.regenRate = energyIO.baseRegen + agility * AGILITY_TO_REGEN_RATIO;
        energyIO.regenRate = std::pow( ENERGY_MAX, agility / attributePointsTotal );

        return energyIO;
    }

    void udpateStats(
        HealthComponent& healthIO,
        EnergyComponent& energyIO,
        AttributesComponent const& attributes
    )
    {
        healthIO = updateFromVitality(
            healthIO,
            attributes.vitality,
            AttributesModule::totalPoints( attributes )
        );

        energyIO = updateFromAgility(
            energyIO,
            attributes.agility,
            AttributesModule::totalPoints( attributes )
        );
    }
}
