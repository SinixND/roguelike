#include "AttributeSystem.h"

#define DEBUG_ENERGY_REGEN

#include "AttributesComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include <cmath>

#if defined( DEBUG ) && defined( DEBUG_ENERGY_REGEN )
#include "Debugger.h"
#endif

namespace AttributeSystem
{
    HealthComponent const& updateFromVitality(
        HealthComponent& healthIO,
        int vitality,
        int attributePointsTotal
    )
    {
        healthIO.maximum = static_cast<int>( healthIO.base + healthIO.base * ( 1.0f * vitality / attributePointsTotal ) );
        healthIO.current = healthIO.maximum;

        return healthIO;
    }

    EnergyComponent const& updateFromAgility(
        EnergyComponent& energyIO,
        int agility,
        int attributePointsTotal
    )
    {
        energyIO.regenRate = ENERGY_REGEN_MIN * std::pow( SPEED_RATIO_MAX, ( 1.0f * agility ) / attributePointsTotal );

#if defined( DEBUG ) && defined( DEBUG_ENERGY_REGEN )
        snx::Debugger::cliLog( "Rate: ", energyIO.regenRate, ", AGI: ", agility, ", TotalPoints: ", attributePointsTotal, "\n" );
#endif

        return energyIO;
    }

    void updateStats(
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
