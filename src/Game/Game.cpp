#include "Game.h"

#include "AISystem.h"
#include "Convert.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Logger.h"
#include "MovementSystem.h"
#include "Objects.h"
#include "PublisherStatic.h"
#include "RenderId.h"
#include "UserInputSystem.h"
#include "World.h"
#include "raylibEx.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <string>

#if defined(DEBUG)
#include "RNG.h"
#endif

void initGame(Game* game)
{
#if defined(DEBUG)
    snx::RNG::seed(1);
#endif

    UserInputSystem::setDefaultInputMappings(game->userInput_);

    //* Setup events
    setupGameEvents(game);

#if defined(DEBUG)
    snx::PublisherStatic::publish(EventId::NEXT_LEVEL);
#endif
}

void setupGameEvents(Game* game)
{
    snx::PublisherStatic::addSubscriber(
        EventId::ACTION_IN_PROGRESS,
        [=]()
        {
            game->actionsInProgress_ = true;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::ACTION_FINISHED,
        [=]()
        {
            game->actionsInProgress_ = false;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::NEXT_LEVEL,
        [=]()
        {
            snx::Logger::log("Entered next level");

            increaseMapLevel(&game->world);

            //* Place Hero on the map entry position
            game->hero.position = Convert::tileToWorld(Vector2I{0, 0});

            snx::PublisherStatic::publish(EventId::HERO_MOVED);
            snx::PublisherStatic::publish(EventId::HERO_POSITION__CHANGED);
            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });

    snx::PublisherStatic::addSubscriber(
        EventId::PREVIOUS_LEVEL,
        [=]()
        {
            snx::Logger::log("Entered previous level");

            decreaseMapLevel(&game->world);

            //* Place Hero on the map exit
            auto const& objects{game->world.currentMap->objects};
            auto const& renderIds{objects.renderIds.values()};
            auto const& positions{objects.positions.values()};

            for (size_t i{0}; i < renderIds.size(); ++i)
            {
                if (renderIds.at(i) == RenderId::DESCEND)
                {
                    game->hero.position = positions.at(i);
                }
            }

            snx::PublisherStatic::publish(EventId::HERO_MOVED);
            snx::PublisherStatic::publish(EventId::HERO_POSITION__CHANGED);
            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });
}

void processUserInput(
    Game* game,
    Cursor& cursor)
{
    //* Take input from mouse, keys or gestures
    //* Continuous movement done by repeating previous input if modifier is active
    UserInputSystem::takeInput(
        game->userInput_,
        cursor.isActive);
}

void updateGameState(
    Game* game,
    GameCamera const& gameCamera,
    Cursor const& cursor)

{

    //* Cycle enemies once to check for action
    bool allEnemiesChecked{false};

    if (
        !game->actionsInProgress_
        && !game->hero.energy.isReady)
    {
        Map& map{*game->world.currentMap};

        allEnemiesChecked = AISystem::checkForAction(
            map.enemies,
            map,
            Convert::worldToTile(game->hero.position),
            game->hero.health,
            gameCamera);
    }

    //* Regenerate energy if no action in progress
    if (
        !game->actionsInProgress_
        && !game->hero.energy.isReady
        && allEnemiesChecked)
    {
        //* Regenerate until one unit becomes ready
        bool isUnitReady{false};

        while (!isUnitReady)
        {
            isUnitReady = regenerate(&game->hero.energy);
            isUnitReady = regenerateAll(&game->world.currentMap->enemies.energies.values()) || isUnitReady;
        }

        ++game->turn_;
        snx::Logger::setStamp(std::to_string(game->turn_));
    }

    //* Trigger potential hero action
    UserInputSystem::triggerAction(
        game->userInput_,
        game->hero,
        cursor,
        *game->world.currentMap,
        gameCamera);

    //* Update hero movement
    MovementSystem::update(
        game->hero.movement,
        game->hero.position,
        game->hero.energy,
        *game->world.currentMap,
        game->hero.position);

    //* Update enemies
    ModuleEnemies::updateEnemies(
        &game->world.currentMap->enemies,
        *game->world.currentMap,
        game->hero.position);
}
