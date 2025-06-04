#include "Game.h"

// #define DEBUG_GAME_LOOP

#include "AISystem.h"
#include "ActionSystem.h"
#include "AttackSystem.h"
#include "AttributeSystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EnergySystem.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "GameCamera.h"
#include "GameState.h"
#include "Hero.h"
#include "InputId.h"
#include "InteractSystem.h"
#include "KillSystem.h"
#include "LevelUpSystem.h"
#include "MoveSystem.h"
#include "Objects.h"
#include "PathSystem.h"
#include "RenderId.h"
#include "VisibilitySystem.h"
#include "WaitSystem.h"
#include "World.h"
#include "raylibEx.h"
#include <Logger.h>
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG )
#if defined( DEBUG_GAME_LOOP )
#include "Debugger.h"
#endif
#include "RNG.h"
#endif

void setupGameEvents(
    Game& game,
    GameCamera const& gameCamera
)
{
    snx::EventDispatcher::addListener(
        EventId::MULTIFRAME_ACTIONS_DONE,
        [&]()
        {
            game.state = GameState::POST_ACTION;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::NEXT_LEVEL,
        [&]()
        {
            snx::Logger::log( "Entered next level\n" );

            game.world = WorldModule::increaseMapLevel( game.world );

            //* Place Hero on the map entry position
            game.hero.position = Convert::tileToWorld( Vector2I{ 0, 0 } );

            snx::EventDispatcher::notify( EventId::HERO_MOVED );
            snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::PREVIOUS_LEVEL,
        [&]()
        {
            snx::Logger::log( "Entered previous level\n" );

            game.world = WorldModule::decreaseMapLevel( game.world );

            //* Place Hero on the map exit
            auto const& objects{ game.world.currentMap->objects };
            auto const& renderIds{ objects.renderIds.values() };
            auto const& positions{ objects.positions.values() };

            for ( size_t idx{ 0 }; idx < renderIds.size(); ++idx )
            {
                if ( renderIds[idx] == RenderId::DESCEND )
                {
                    game.hero.position = positions[idx];
                }
            }

            snx::EventDispatcher::notify( EventId::HERO_MOVED );
            snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::HERO_POSITION_CHANGED,
        [&]()
        {
            //* VisibilitySystem
            game.world.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                game.world.currentMap->tiles,
                game.world.currentMap->fogs,
                GameCameraModule::viewportInTiles( gameCamera ),
                Convert::worldToTile( game.hero.position ),
                game.hero.visionRange
            );
        },
        true
    );

    snx::EventDispatcher::addListener(
        EventId::INTERRUPT_MOVEMENT,
        [&]()
        {
            game.hero.path.clear();
        }
    );

    snx::EventDispatcher::addListener(
        EventId::NEXT_TURN,
        [&]()
        {
            ++game.turn;
            snx::Logger::incrementTurn();
            snx::Logger::updateHistory();

            game.state = GameState::ACTION_HERO;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::NPC_ACTION,
        [&]()
        {
            game.state = GameState::ACTION_NPC;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::LEVEL_UP,
        [&]()
        {
            game.state = GameState::LEVEL_UP;
        }
    );

    snx::EventDispatcher::addListener(
        EventId::LEVELED_UP,
        [&]()
        {
            game.state = GameState::REGEN;
        }
    );
}

namespace GameModule
{
    Game const& init(
        Game& game,
        GameCamera const& gameCamera
    )
    {
#if defined( DEBUG )
        snx::RNG::seed( 1 );
#endif
        game.hero = Hero{};

        AttributeSystem::updateStats(
            game.hero.health,
            game.hero.energy,
            game.hero.attributes
        );

        game.world = World{};

        game.turn = 1;

        setupGameEvents(
            game,
            gameCamera
        );

#if defined( DEBUG )
        // snx::EventDispatcher::notify( EventId::NEXT_LEVEL );
#endif

        return game;
    }

    Game const& update(
        Game& game,
        GameCamera const& gameCamera,
        Cursor const& cursor,
        InputId currentInputId,
        float dt
    )
    {
        switch ( game.state )
        {
            default:
            case GameState::REGEN:
            {
                EnergySystem::udpate(
                    game.hero.energy,
                    game.hero.isIdle,
                    game.world.currentMap->enemies.energies,
                    game.world.currentMap->enemies.isIdles
                );

                break;
            }

            case GameState::ACTION_HERO:
            {
                ActionSystem::update(
                    game.hero,
                    *game.world.currentMap,
                    currentInputId,
                    cursor,
                    gameCamera
                );

                if ( !game.hero.isIdle )
                {
                    game.state = GameState::SINGLE_FRAME_ACTIONS;
                }

                break;
            }

            case GameState::ACTION_NPC:
            {
                AISystem::update(
                    game.world.currentMap->enemies,
                    *game.world.currentMap,
                    game.hero,
                    gameCamera
                );

                game.state = GameState::SINGLE_FRAME_ACTIONS;

                break;
            }

            case GameState::SINGLE_FRAME_ACTIONS:
            {
                WaitSystem::update(
                    game.hero,
                    game.world.currentMap->enemies
                );

                AttackSystem::update(
                    game.hero,
                    game.world.currentMap->enemies
                );

                InteractSystem::update(
                    game.hero,
                    game.world.currentMap->objects
                );

                game.state = GameState::MULTI_FRAME_ACTIONS;

                break;
            }

            case GameState::MULTI_FRAME_ACTIONS:
            {
                MoveSystem::update(
                    game.hero,
                    game.world.currentMap->enemies,
                    dt
                );

                break;
            }

            case GameState::POST_ACTION:
            {
                PathSystem::update( game.hero );

                KillSystem::update(
                    game.hero,
                    game.world.currentMap->enemies
                );

                game.world.currentMap->enemies = EnemiesModule::replaceDead(
                    game.world.currentMap->enemies,
                    game.world.currentMap->tiles,
                    game.world.currentMapLevel
                );

                if ( game.state == GameState::POST_ACTION )
                {
                    game.state = GameState::REGEN;
                }

                break;
            }

            case GameState::LEVEL_UP:
            {
                game = LevelUpSystem::update(
                    game,
                    currentInputId
                );

                break;
            }
        }

        return game;
    }
}
