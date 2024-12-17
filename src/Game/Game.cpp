#include "Game.h"

#include "AISystem.h"
#include "Cursor.h"
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
    //* Determine next actor (in input step!)
    //* Update next actor
    //* (Check hero?)
    //* Regen

    //==================================

    //* Cycle enemies once to check for action
    bool allEnemiesChecked{false};

    //* Trigger AI action
    if (
        !actionInProgress_
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
        !actionInProgress_
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

        ++turn_;
        snx::Logger::setStamp(std::to_string(turn_));
    }

    //* Trigger potential hero action
    // UserInputSystem::triggerAction(
    //     userInput_,
    //     hero,
    //     cursor,
    //     *world.currentMap,
    //     gameCamera);
    {
        if (!hero.energy.isReady())
        {
            return;
        }

        if (userInputComponent.inputAction == InputActionId::NONE)
        {
            //* Trigger input agnostic actions, eg. non-empty path
            MovementSystem::prepareInputAgnostic(
                hero.movement,
                hero.transform,
                hero.position);

            return;
        }

        switch (userInputComponent.inputAction)
        {
            case InputActionId::ACT_UP:
            {
                Vector2I target{
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::up)};

                if (map.enemies.ids.contains(target))
                {
                    performAttack(hero, map, target);

                    break;
                }

                MovementSystem::prepareByDirection(
                    hero.movement,
                    hero.position,
                    Directions::up);

                break;
            }

            case InputActionId::ACT_LEFT:
            {
                Vector2I target{
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::left)};

                if (map.enemies.ids.contains(target))
                {
                    performAttack(hero, map, target);

                    break;
                }

                MovementSystem::prepareByDirection(
                    hero.movement,
                    hero.position,
                    Directions::left);

                break;
            }

            case InputActionId::ACT_DOWN:
            {
                Vector2I target{
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::down)};

                if (map.enemies.ids.contains(target))
                {
                    performAttack(hero, map, target);

                    break;
                }

                MovementSystem::prepareByDirection(
                    hero.movement,
                    hero.position,
                    Directions::down);

                break;
            }

            case InputActionId::ACT_RIGHT:
            {
                Vector2I target{
                    Vector2Add(
                        hero.position.tilePosition(),
                        Directions::right)};

                if (map.enemies.ids.contains(target))
                {
                    performAttack(hero, map, target);

                    break;
                }

                MovementSystem::prepareByDirection(
                    hero.movement,
                    hero.position,
                    Directions::right);

                break;
            }

            case InputActionId::MOVE_TO_TARGET:
            {
                MovementSystem::prepareByNewPath(
                    hero.movement,
                    hero.position,
                    PathfinderSystem::findPath(
                        map,
                        hero.position.tilePosition(),
                        cursor.position.tilePosition(),
                        gameCamera));

                break;
            }

            case InputActionId::ACT_IN_PLACE:
            {
                Vector2I heroTilePosition{hero.position.tilePosition()};

                //* Wait if nothing to interact
                if (!map.objects.events.contains(heroTilePosition))
                {
                    snx::Logger::log("Hero waits...");

                    hero.energy.consume();

                    regenerate(hero.health);

                    break;
                }

                snx::PublisherStatic::publish(map.objects.events.at(heroTilePosition));

                break;
            }

            case InputActionId::TOGGLE_CURSOR:
            {
                snx::PublisherStatic::publish(EventId::CURSOR_TOGGLE);

                break;
            }

            default:
                break;
        }

        //* Reset
        userInputComponent.resetInputAction();
    }

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

int Game::turn() const
{
    return turn_;
}
