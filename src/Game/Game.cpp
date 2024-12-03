#include "Game.h"

#include "AISystem.h"
#include "Cursor.h"
#include "Enemies.h"
#include "EventId.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Logger.h"
#include "MovementSystem.h"
#include "Objects.h"
#include "PositionComponent.h"
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

void Game::init()
{
#if defined(DEBUG)
    snx::RNG::seed(1);
#endif

    UserInputSystem::setDefaultInputMappings(userInput_);

    //* Setup events
    setupGameEvents();

#if defined(DEBUG)
    snx::PublisherStatic::publish(EventId::NEXT_LEVEL);
#endif
}

void Game::setupGameEvents()
{
    snx::PublisherStatic::addSubscriber(
        EventId::ACTION_IN_PROGRESS,
        [&]()
        {
            actionInProgress_ = true;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::ACTION_FINISHED,
        [&]()
        {
            actionInProgress_ = false;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::NEXT_LEVEL,
        [&]()
        {
            snx::Logger::log("Entered next level");

            world.increaseMapLevel();

            //* Place Hero on the map entry position
            hero.position.changeTo(Vector2I{0, 0});

            snx::PublisherStatic::publish(EventId::HERO_MOVED);
            snx::PublisherStatic::publish(EventId::HERO_POSITION_CHANGED);
            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });

    snx::PublisherStatic::addSubscriber(
        EventId::PREVIOUS_LEVEL,
        [&]()
        {
            snx::Logger::log("Entered previous level");

            world.decreaseMapLevel();

            //* Place Hero on the map exit
            auto const& objects{world.currentMap->objects};
            auto const& renderIds{objects.renderIds.values()};
            auto const& positions{objects.positions.values()};

            for (size_t i{0}; i < renderIds.size(); ++i)
            {
                if (renderIds.at(i) == RenderId::DESCEND)
                {
                    hero.position.changeTo(
                        positions.at(i).tilePosition());
                }
            }

            snx::PublisherStatic::publish(EventId::HERO_MOVED);
            snx::PublisherStatic::publish(EventId::HERO_POSITION_CHANGED);
            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });
}

void Game::processInput(Cursor& cursor)
{
    //* Take input from mouse, keys or gestures
    //* Continuous movement done by repeating previous input if modifier is active
    UserInputSystem::takeInput(
        userInput_,
        cursor.isActive);
}

void Game::updateState(
    GameCamera const& gameCamera,
    Cursor const& cursor)
{
    //* Cycle enemies once to check for action
    bool allEnemiesChecked{false};

    //* Trigger AI action
    if (
        !actionInProgress_
        && !hero.energy.isReady())
    {
        Map& map{*world.currentMap};

        allEnemiesChecked = AISystem::checkForAction(
            map.enemies,
            map,
            hero.position.tilePosition(),
            hero.health,
            gameCamera);
    }

    //* Regenerate energy if no action in progress
    if (
        !actionInProgress_
        && !hero.energy.isReady()
        && allEnemiesChecked)
    {
        //* Regenerate until one unit becomes ready
        bool isUnitReady{false};

        while (!isUnitReady)
        {
            isUnitReady = hero.energy.regenerate();
            isUnitReady = regenerateAll(&world.currentMap->enemies) || isUnitReady;
        }

        ++turn_;
        snx::Logger::setStamp(std::to_string(turn_));
    }

    //* Trigger potential hero action
    UserInputSystem::triggerAction(
        userInput_,
        hero,
        cursor,
        *world.currentMap,
        gameCamera);

    //* Update hero movement
    MovementSystem::update(
        hero.transform,
        hero.position,
        hero.energy,
        *world.currentMap,
        hero.position);

    //* Update enemies
    updateEnemies(
        &world.currentMap->enemies,
        *world.currentMap,
        hero.position);
}

int Game::turn() const
{
    return turn_;
}
