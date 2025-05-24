#include "Hero.h"

// #define DEBUG_HERO_ACTIONS

#include "CollisionSystem.h"
#include "CombatSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Directions.h"
#include "EventDispatcher.h"
#include "ExperienceComponent.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputId.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
#include "Debugger.h"
#endif

[[nodiscard]]
Hero const& processDirectionalInput(
    Hero& hero,
    Map& mapIO,
    Vector2I const& direction
)
{
    Vector2I target{
        Vector2Add(
            Convert::worldToTile( hero.position ),
            direction
        )
    };

    //* Attack
    if ( mapIO.enemies.ids.contains( target ) )
    {
        size_t enemyIdx{ mapIO.enemies.ids.index( target ) };

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
        snx::Debugger::cliLog( "Hero attacks.\n" );
#endif
        CombatSystem::performAttack(
            hero.damage,
            mapIO.enemies.healths[enemyIdx]
        );

        if ( mapIO.enemies.healths[enemyIdx].current > 0 )
        {
            return hero;
        }

        if (
            ExperienceModule::getExpValue(
                mapIO.enemies.experiences[enemyIdx].level,
                hero.experience.level
            )
            > ( hero.experience.levelUpThreshold - hero.experience.current )
        )
        {
            snx::EventDispatcher::notify( EventId::LEVEL_UP );
        }

        hero.experience = ExperienceModule::gainExp(
            hero.experience,
            mapIO.enemies.experiences[enemyIdx].level
        );

        return hero;
    }
    //* Move
    else if ( !CollisionSystem::checkCollisionForHero(
                  mapIO.tiles,
                  mapIO.enemies,
                  target
              ) )
    {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
        snx::Debugger::cliLog( "Hero moves.\n" );
#endif
        hero.energy = EnergyModule::exhaust( hero.energy );

        hero.transform = MovementSystem::prepareByDirection(
            hero.transform,
            hero.movement,
            direction
        );
    }

    return hero;
}

namespace HeroModule
{
    Hero const& executeAction(
        Hero& hero,
        Map& mapIO,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInput
    )
    {
        switch ( currentInput )
        {
            default:
            case InputId::NONE:
            {
                //* Continue multi-frame action
                if ( !hero.movement.path.empty() )
                {
                    //* Move
                    if ( !CollisionSystem::checkCollisionForHero(
                             mapIO.tiles,
                             mapIO.enemies,
                             hero.movement.path.rbegin()[1]
                         ) )
                    {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
                        snx::Debugger::cliLog( "Hero moves.\n" );
#endif
                        hero.energy = EnergyModule::exhaust( hero.energy );

                        hero.transform = MovementSystem::prepareFromExistingPath(
                            hero.transform,
                            hero.movement
                        );
                    }
                    //* Stop
                    else
                    {
                        hero.transform = MovementSystem::resetTransform( hero.transform );
                        hero.movement.path.clear();
                    }
                }

                break;
            }

            case InputId::ACT_UP:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::up
                );

                break;
            }

            case InputId::ACT_LEFT:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::left
                );

                break;
            }

            case InputId::ACT_DOWN:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::down
                );

                break;
            }

            case InputId::ACT_RIGHT:
            {
                hero = processDirectionalInput(
                    hero,
                    mapIO,
                    Directions::right
                );

                break;
            }

            //* NOTE: Fall through possible to ACT_IN_PLACE if cursor.position == hero.position
            case InputId::ACT_TO_TARGET:
            {
                std::vector<Vector2I> path{ PathfinderSystem::calculateAStarPath(
                    Convert::worldToTile( hero.position ),
                    Convert::worldToTile( cursor.position ),
                    mapIO,
                    gameCamera
                ) };

                if (
                    Convert::worldToTile( hero.position )
                    == Convert::worldToTile( cursor.position )
                )
                {
                    //* NOTE: Fallthrough
                }

                else if ( path.empty() )
                {
                    break;
                }

                //* Check if an enemy is at the first path tile
                else if ( mapIO.enemies.ids.contains( path.rbegin()[1] ) )
                {
                    size_t enemyIdx{ mapIO.enemies.ids.at( path.rbegin()[1] ) };

                    CombatSystem::performAttack(
                        hero.damage,
                        mapIO.enemies.healths[enemyIdx]
                    );

                    path.clear();

                    break;
                }

                else if ( !CollisionSystem::checkCollisionForHero(
                              mapIO.tiles,
                              mapIO.enemies,
                              path.rbegin()[1]
                          ) )
                {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
                    snx::Debugger::cliLog( "Hero moves.\n" );
#endif
                    hero.energy = EnergyModule::exhaust( hero.energy );

                    hero.movement = MovementSystem::prepareByNewPath(
                        hero.movement,
                        path
                    );

                    hero.transform = MovementSystem::prepareFromExistingPath(
                        hero.transform,
                        hero.movement
                    );

                    break;
                }

                else
                {
                    path.clear();

                    break;
                }
                //* NOTE: Possible fallthrough to ACT_TO_TARGET
            }

            //* NOTE: Possible fallthrough from ACT_TO_TARGET
            case InputId::ACT_IN_PLACE:
            {
                //* Interact
                if ( mapIO.objects.ids.contains( Convert::worldToTile( hero.position ) ) )
                {
                    size_t objectId{ mapIO.objects.ids.at( Convert::worldToTile( hero.position ) ) };

                    //* Interact if possible
                    if ( mapIO.objects.eventIds.contains( objectId ) )
                    {
#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
                        snx::Debugger::cliLog( "Hero interacts.\n" );
#endif
                        hero.energy = EnergyModule::exhaust( hero.energy );

                        snx::EventDispatcher::notify( mapIO.objects.eventIds.at( objectId ) );

                        break;
                    }
                }

#if defined( DEBUG ) && defined( DEBUG_HERO_ACTIONS )
                snx::Debugger::cliLog( "Hero waits.\n" );
#endif
                snx::Logger::log( "Hero waits...\n" );

                hero.energy = EnergyModule::exhaust( hero.energy );

                hero.health = HealthModule::regenerate( hero.health );

                break;
            }
        }

        return hero;
    }
}
