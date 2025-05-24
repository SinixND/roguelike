#include "AttackSystem.h"

#include "CombatSystem.h"
#include "Enemies.h"
#include "Hero.h"

namespace AttackSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO
    )
    {
        if ( heroIO.attack )
        {
            heroIO.energy = EnergyModule::exhaust( heroIO.energy );

            CombatSystem::performAttack(
                heroIO.damage,
                enemiesIO.healths.at(
                    enemiesIO.ids.at(
                        heroIO.attack->target
                    )
                )
            );

            heroIO.attack.reset();
        }

        for ( size_t idx{ 0 }; idx < enemiesIO.attacks.size(); ++idx )
        {
            size_t enemyId{ enemiesIO.attacks.key( idx ) };

            enemiesIO.energies.at( enemyId ) = EnergyModule::exhaust( enemiesIO.energies.at( enemyId ) );

            CombatSystem::performAttack(
                enemiesIO.damages.at( enemyId ),
                heroIO.health
            );
        }

        enemiesIO.attacks.clear();
    }
}
