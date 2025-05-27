#include "KillSystem.h"

#include "Enemies.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Hero.h"

namespace KillSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO
    )
    {
        if ( heroIO.health.current <= 0 )
        {
            snx::EventDispatcher::notify( EventId::GAME_OVER );
        }

        for ( size_t idx{ 0 }; idx < enemiesIO.healths.size(); ++idx )
        {
            size_t enemyId{ enemiesIO.healths.key( idx ) };

            if ( enemiesIO.healths.at( enemyId ).current > 0 )
            {
                continue;
            }

            //* Only hero can damage an enemy _at the moment_, no check for killerId necessary
            else // if ( enemiesIO.healths.at( enemyId ).lastDamageSourceId == 1 ) // Hero
            {
                if (
                    ExperienceModule::getExpValue(
                        enemiesIO.experiences.at( enemyId ).level,
                        heroIO.experience.level
                    )
                    > ( heroIO.experience.levelUpThreshold - heroIO.experience.current )
                )
                {
                    snx::EventDispatcher::notify( EventId::LEVEL_UP );
                }

                heroIO.experience = ExperienceModule::gainExp(
                    heroIO.experience,
                    enemiesIO.experiences.at( enemyId ).level
                );
            }
        }
    }
}
