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
            CombatSystem::performAttack(
                heroIO.damage,
                1,
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

            CombatSystem::performAttack(
                enemiesIO.damages.at( enemyId ),
                enemyId,
                heroIO.health
            );
        }

        enemiesIO.attacks.clear();
    }
}
