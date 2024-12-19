#include "Game.h"

#include "AISystem.h"
#include "Cursor.h"
#include "DamageSystem.h"
#include "Enemies.h"
#include "EventId.h"
#include "GameCamera.h"
#include "Hero.h"
#include "InputActionId.h"
#include "InputHandler.h"
#include "InputMappings.h"
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

    inputMappings = defaultInputMappings();

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
            actionInProgress = true;
        });

    snx::PublisherStatic::addSubscriber(
        EventId::ACTION_FINISHED,
        [&]()
        {
            actionInProgress = false;
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

void Game::prepare(Cursor& cursor)
{
    //* Take input from mouse, keys or gestures
    //* Continuous movement done by repeating previous input if modifier is active
    inputAction = checkKeyboard(
        inputHandler,
        inputMappings);

    if (inputAction != InputActionId::NONE)
    {
        return;
    }

    inputAction = checkMouse(inputMappings, cursor.isActive);

    if (inputAction != InputActionId::NONE)
    {
        return;
    }

    inputAction = checkGesture(inputHandler);
}

void Game::update(
    GameCamera const& gameCamera,
    Cursor const& cursor)
{
    //* Determine next actor (in input step? use/update AI-current turn)

    //* Update next actor
    //* (Check hero?)
    //* Regen

    //* AI
    if (!actionInProgress)
    {
        Enemies& enemies{world.currentMap->enemies};

        size_t activeEnemyId = getActiveEnemy(
            enemies.energies,
            enemies.ais,
            turn);

        if (activeEnemyId)
        {
            actionInProgress = AISystem::takeAction(
                enemies.ais.at(activeEnemyId),
                enemies.positions.at(activeEnemyId),
                enemies.movements.at(activeEnemyId),
                enemies.energies.at(activeEnemyId),
                enemies.damages.at(activeEnemyId),
                *world.currentMap,
                hero.position.tilePosition(),
                hero.health,
                gameCamera);
        }
    }

    //* Update multi-frame actions
    if (actionInProgress)
    {
    }
    //==================================

    //* Cycle enemies once to check for action
    bool allEnemiesChecked{false};

    //* Trigger AI action
    if (
        !actionInProgress
        && !hero.energy.isReady())
    {
        Map& map{*world.currentMap};

        allEnemiesChecked = AISystem::checkReadiness(
            map.enemies,
            map,
            hero.position.tilePosition(),
            hero.health,
            gameCamera);
    }

    //* Regenerate energy if no action in progress
    if (
        !actionInProgress
        && !hero.energy.isReady()
        && allEnemiesChecked)
    {
        //* Regenerate until one unit becomes ready
        bool isUnitReady{false};

        while (!isUnitReady)
        {
            isUnitReady = hero.energy.regenerate();
            isUnitReady = regenerateAll(world.currentMap->enemies) || isUnitReady;
        }

        ++turn;
        snx::Logger::setStamp(std::to_string(turn));
    }

    //* Trigger potential hero action
    UserInputSystem::takeAction(
        inputAction,
        hero,
        cursor,
        *world.currentMap,
        gameCamera);

    inputAction = InputActionId::NONE;

    //* Update hero movement
    MovementSystem::update(
        hero.transform,
        hero.movement,
        hero.position,
        hero.energy,
        *world.currentMap,
        hero.position);

    //* Update enemies
    updateEnemies(
        world.currentMap->enemies,
        *world.currentMap,
        hero.position);
}

