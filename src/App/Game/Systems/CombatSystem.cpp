#include "CombatSystem.h"

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"

namespace CombatSystem
{
    void performAttack(
        EnergyComponent& attackerEnergyIO,
        DamageComponent const& attackerDamage,
        HealthComponent& defenderHealthIO
    )
    {
        attackerEnergyIO = EnergyModule::consume(
            attackerEnergyIO,
            attackerDamage.costMultiplier
        );

        defenderHealthIO = HealthModule::damage(
            defenderHealthIO,
            DamageModule::damageRNG( attackerDamage )
        );

        return;
    }
}
