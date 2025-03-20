#include "CombatSystem.h"

#include "EventDispatcher.h"
#include "EventId.h"
#include "Hero.h"
#include "Map.h"
#include "raylibEx.h"
#include <cstddef>

namespace CombatSystem
{
    void performAttack(
        Hero& heroIO,
        Map& mapIO,
        Vector2I const& target
    )
    {
        size_t enemyIdx{ mapIO.enemies.ids.index( target ) };

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
        snx::Debugger::cliLog( "Hero attacks.\n" );
#endif
        heroIO.energy = EnergyModule::consume(
            heroIO.energy,
            heroIO.damage.costMultiplier
        );

        HealthModule::damage(
            mapIO.enemies.healths[mapIO.enemies.ids.index( target )],
            DamageModule::damageRNG( heroIO.damage )
        );

        if ( mapIO.enemies.healths[enemyIdx].current > 0 )
        {
            return;
        }

        if (
            ( heroIO.experience.levelUpThreshold - heroIO.experience.current )
            <= ExperienceModule::getExpValue(
                mapIO.enemies.experiences[enemyIdx].level,
                heroIO.experience.level
            )
        )
        {
            snx::EventDispatcher::notify( EventId::LEVEL_UP );
        }

        heroIO.experience = ExperienceModule::gainExp(
            heroIO.experience,
            mapIO.enemies.experiences[enemyIdx].level
        );

        return;
    }
}
