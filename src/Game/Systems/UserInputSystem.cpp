#include "UserInputSystem.h"

//* #define DEBUG_GESTURES
//* #define DEBUG_GESTURE_EVENTS

#include "CollisionSystem.h"
#include "Cursor.h"
#include "DamageSystem.h"
#include "Directions.h"
#include "EventId.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "InputActionId.h"
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
    Enemies& enemies,
    Vector2I const& target)
{
    snx::Logger::log("Hero deals ");

    DamageSystem::attack(
        hero.damage,
        enemies.healths.at(
            enemies.ids.at(
                target)));
}

bool processDirectionalInput(
    Hero& hero,
    Map& map,
    Vector2I direction)
{
    bool isActionMultiFrame = false;

    Vector2I target{
        Vector2Add(
            hero.position.tilePosition(),
            direction)};

    if (map.enemies.ids.contains(target))
    {
        performAttack(
            hero,
            map.enemies,
            target);

        hero.energy.consume();
    }
    else if (!CollisionSystem::checkCollision(
                 map.tiles,
                 map.enemies,
                 target,
                 hero.position.tilePosition()))
    {
        MovementSystem::prepareByDirection(
            hero.movement,
            direction,
            hero.transform);

        hero.energy.consume();

        isActionMultiFrame = true;
    }

    return isActionMultiFrame;
}

bool UserInputSystem::takeAction(
    InputActionId inputAction,
    Hero& hero,
    Cursor const& cursor,
    Map& map,
    GameCamera const& gameCamera)
{
    bool isActionMultiFrame{false};

    switch (inputAction)
    {
        default:
        case InputActionId::NONE:
        {
            // if (!hero.transform.speed
            //     && !hero.movement.path.empty())
            // {
            //     MovementSystem::prepareFromExistingPath(
            //         hero.movement,
            //         // hero.position);
            //         hero.transform);
            //
            //     isActionMultiFrame = true;
            // }

            break;
        }

        case InputActionId::ACT_UP:
        {
            isActionMultiFrame = processDirectionalInput(
                hero,
                map,
                Directions::up);

            break;
        }

        case InputActionId::ACT_LEFT:
        {
            isActionMultiFrame = processDirectionalInput(
                hero,
                map,
                Directions::left);

            break;
        }

        case InputActionId::ACT_DOWN:
        {
            isActionMultiFrame = processDirectionalInput(
                hero,
                map,
                Directions::down);

            break;
        }

        case InputActionId::ACT_RIGHT:
        {
            isActionMultiFrame = processDirectionalInput(
                hero,
                map,
                Directions::right);

            break;
        }

        case InputActionId::MOVE_TO_TARGET:
        {
            std::vector<Vector2I> path{PathfinderSystem::findPath(
                map,
                hero.position.tilePosition(),
                cursor.position.tilePosition(),
                gameCamera)};

            if (!path.empty())
            {
                MovementSystem::prepareByNewPath(
                    hero.movement,
                    path);
            }

            isActionMultiFrame = !path.empty();

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
    }

    inputAction = InputActionId::NONE;

    return isActionMultiFrame;
}
