#include "UserInputSystem.h"

//* #define DEBUG_GESTURES
//* #define DEBUG_GESTURE_EVENTS

#include "Cursor.h"
#include "DamageSystem.h"
#include "Directions.h"
#include "EventId.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputActionId.h"
#include "InputHandler.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void performAttack(
    Hero& hero,
    Map& map,
    Vector2I const& target)
{
    snx::Logger::log("Hero deals ");

    DamageSystem::attack(
        hero.damage,
        map.enemies.healths.at(
            map.enemies.ids.at(
                target)));

    hero.energy.consume();
}

void UserInputSystem::triggerAction(
    InputHandler& userInputComponent,
    Hero& hero,
    Cursor const& cursor,
    Map& map,
    GameCamera const& gameCamera)
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
