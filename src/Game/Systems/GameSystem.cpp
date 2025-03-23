#include "GameSystem.h"

#include "AISystem.h"
#include "Convert.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Game.h"
#include "Logger.h"
#include "MovementSystem.h"

[[nodiscard]]
Hero const& continueHeroMovement(
    Hero& hero,
    float dt
)
{
    Vector2I oldPosition{ Convert::worldToTile( hero.position ) };

    hero.position = MovementSystem::updateSingle(
        hero.position,
        hero.transform,
        dt
    );

    snx::EventDispatcher::notify( EventId::HERO_MOVED );

    if ( oldPosition != Convert::worldToTile( hero.position ) )
    {
        snx::EventDispatcher::notify( EventId::HERO_POSITION_CHANGED );
    }

    return hero;
}

[[nodiscard]]
Enemies const& continueEnemyMovements(
    Enemies& enemies,
    float dt
)
{
    Vector2I oldPosition{};

    for ( size_t idx{ 0 }; idx < enemies.transforms.size(); ++idx )
    {
        oldPosition = Convert::worldToTile( enemies.positions[idx] );

        //* Update movement
        enemies.positions[idx] = MovementSystem::updateSingle(
            enemies.positions[idx],
            enemies.transforms[idx],
            dt
        );

        //* Update ids_ key if tilePosition changed
        if ( oldPosition != Convert::worldToTile( enemies.positions[idx] ) )
        {
            enemies.ids.changeKey(
                oldPosition,
                Convert::worldToTile( enemies.positions[idx] )
            );
        }
    }

    return enemies;
}

[[nodiscard]]
Game const& continueMultiFrameActions(
    Game& game,
    float dt
)
{
    //* Hero
    if ( game.hero.transform.remainingDistance )
    {
        game.hero = continueHeroMovement(
            game.hero,
            dt
        );
    }
    //* Enemies
    else
    {
        game.world.currentMap->enemies = continueEnemyMovements(
            game.world.currentMap->enemies,
            dt
        );
    }

    return game;
}

[[nodiscard]]
Game const& executeInstantActions(
    Game& game,
    GameCamera const& gameCamera,
    Cursor const& cursor,
    InputId currentInputId
)
{
    //* Hero
    if ( game.hero.energy.isReady )
    {
        game.hero = HeroModule::executeAction(
            game.hero,
            *game.world.currentMap,
            cursor,
            gameCamera,
            currentInputId
        );
    }

    //* Enemies
    else
    {
        game.world.currentMap->enemies = AISystem::executeNextAction(
            game.world.currentMap->enemies,
            game.activeEnemyId,
            game.hero,
            *game.world.currentMap,
            gameCamera
        );
    }

    return game;
}

[[nodiscard]]
Game const& updateDefault(
    Game& game,
    GameCamera const& gameCamera,
    Cursor const& cursor,
    InputId currentInputId,
    float dt
)
{
    if ( !game.isMultiFrameActionActive )
    {
        game = executeInstantActions(
            game,
            gameCamera,
            cursor,
            currentInputId
        );
    }

    if ( game.isMultiFrameActionActive )
    {
        game = continueMultiFrameActions(
            game,
            dt
        );

        return game;
    }

    //* End turn
    if ( game.hero.health.current <= 0 )
    {
        snx::EventDispatcher::notify( EventId::GAME_OVER );
    }

    game.world.currentMap->enemies = EnemiesModule::replaceDead(
        game.world.currentMap->enemies,
        game.world.currentMap->tiles,
        game.world.currentMapLevel
    );

    //* Skip energy regeneration while a unit is ready
    if ( game.hero.energy.isReady
         || game.activeEnemyId )
    {
        return game;
    }

    //* Regenerate energy until a unit becomes ready
    bool isUnitReady{ false };

    //* Regenerate until one unit becomes ready
#if defined( DEBUG ) && defined( DEBUG_GAME_LOOP )
    snx::Debugger::cliLog( "No action left. Regen units.\n" );
#endif
    while ( !isUnitReady )
    {
        //* Unit is ready when regenerate is _not_ successful
        isUnitReady = EnergyModule::regenerate( game.hero.energy );
        isUnitReady |= EnemiesModule::regenerate( game.world.currentMap->enemies.energies );
    }

    //* Increment turn when hero is ready
    if ( game.hero.energy.isReady )
    {
#if defined( DEBUG ) && defined( DEBUG_GAME_LOOP )
        snx::Debugger::cliLog( "Hero ready. Next Turn\n\n" );
#endif
        snx::EventDispatcher::notify( EventId::NEXT_TURN );

        snx::Logger::incrementTurn();
    }

    return game;
}

namespace GameSystem
{
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
            case GameState::DEFAULT:
            {
                game = updateDefault(
                    game,
                    gameCamera,
                    cursor,
                    currentInputId,
                    dt
                );
                break;
            }

            case GameState::LEVEL_UP:
            {
                //* SceneGameOver handles input because it can be a UI selection
                break;
            }
        }

        return game;
    }
}
