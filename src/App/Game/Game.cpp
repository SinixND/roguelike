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
            // game.state = GameState::POST_ACTION;
            game.state = GameState::ACTION_HERO;
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
        Game& gameIO,
        GameCamera const& gameCamera
    )
    {
#if defined( DEBUG )
        snx::RNG::seed( 1 );
#endif
        gameIO.hero = Hero{};

        AttributeSystem::updateStats(
            gameIO.hero.health,
            gameIO.hero.energy,
            gameIO.hero.attributes
        );

        gameIO.world = World{};

        gameIO.turn = 1;

        setupGameEvents(
            gameIO,
            gameCamera
        );

#if defined( DEBUG )
        // snx::EventDispatcher::notify( EventId::NEXT_LEVEL );
#endif

        return gameIO;
    }

    Game const& update(
        Game& gameIO,
        GameCamera const& gameCamera,
        Cursor const& cursor,
        InputId currentInputId,
        float dt
    )
    {
        switch ( gameIO.state )
        {
            default:
            case GameState::REGEN:
            {
                EnergySystem::udpate(
                    gameIO.hero.energy,
                    gameIO.hero.awaitsInput,
                    gameIO.world.currentMap->enemies.energies,
                    gameIO.world.currentMap->enemies.isIdles
                );

                if ( gameIO.hero.awaitsInput )
                {
                    gameIO.state = GameState::MULTI_FRAME_ACTIONS;
                }

                break;
            }

            case GameState::ACTION_HERO:
            {
                ActionSystem::update(
                    gameIO.hero,
                    *gameIO.world.currentMap,
                    currentInputId,
                    cursor,
                    gameCamera
                );

                if ( !gameIO.hero.awaitsInput )
                {
                    gameIO.state = GameState::SINGLE_FRAME_ACTIONS;
                }

                break;
            }

            case GameState::ACTION_NPC:
            {
                AISystem::update(
                    gameIO.world.currentMap->enemies,
                    *gameIO.world.currentMap,
                    gameIO.hero,
                    gameCamera
                );

                gameIO.state = GameState::SINGLE_FRAME_ACTIONS;

                break;
            }

            case GameState::SINGLE_FRAME_ACTIONS:
            {
                WaitSystem::update(
                    gameIO.hero,
                    gameIO.world.currentMap->enemies
                );

                AttackSystem::update(
                    gameIO.hero,
                    gameIO.world.currentMap->enemies
                );

                InteractSystem::update(
                    gameIO.hero,
                    gameIO.world.currentMap->objects
                );

                // gameIO.state = GameState::MULTI_FRAME_ACTIONS;
                gameIO.state = GameState::POST_ACTION;

                break;
            }

            case GameState::MULTI_FRAME_ACTIONS:
            {
                MoveSystem::update(
                    gameIO.hero,
                    gameIO.world.currentMap->enemies,
                    dt
                );

                break;
            }

            case GameState::POST_ACTION:
            {
                PathSystem::update( gameIO.hero );

                KillSystem::update(
                    gameIO.hero,
                    gameIO.world.currentMap->enemies
                );

                gameIO.world.currentMap->enemies = EnemiesModule::replaceDead(
                    gameIO.world.currentMap->enemies,
                    gameIO.world.currentMap->tiles,
                    gameIO.world.currentMapLevel
                );

                //* TODO: Delete redundant check?
                // if ( gameIO.state == GameState::POST_ACTION )
                // {
                gameIO.state = GameState::REGEN;
                // }

                break;
            }

            case GameState::LEVEL_UP:
            {
                gameIO = LevelUpSystem::update(
                    gameIO,
                    currentInputId
                );

                break;
            }
        }

        return gameIO;
    }
}
