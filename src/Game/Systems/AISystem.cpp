#include "AISystem.h"

#include "CollisionSystem.h"
#include "DamageComponent.h"
#include "DamageSystem.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Hero.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include "TransformComponent.h"
#include "World.h"
#include <cstddef>
#include <vector>

bool AISystem::takeActions(
    World& world,
    Hero& hero,
    size_t& activeEnemyId,
    GameCamera const& gameCamera,
    int turn)
{
    bool multiFrameActionActive{false};

    Enemies& enemies{world.currentMap->enemies};

    do
    {
        activeEnemyId = getActiveEnemy(
            enemies.energies,
            enemies.ais,
            turn);

        if (activeEnemyId)
        {
            enemies.ais[activeEnemyId].turn = turn;

            multiFrameActionActive |= AISystem::takeAction(
                enemies.ais.at(activeEnemyId),
                enemies.positions.at(activeEnemyId),
                enemies.movements.at(activeEnemyId),
                enemies.transforms.at(activeEnemyId),
                enemies.energies.at(activeEnemyId),
                enemies.damages.at(activeEnemyId),
                *world.currentMap,
                hero.position.tilePosition(),
                hero.health,
                gameCamera);
        }
    } while (activeEnemyId);

    return multiFrameActionActive;
}

bool AISystem::takeAction(
    AIComponent const& ai,
    PositionComponent& position,
    MovementComponent& movement,
    TransformComponent& transform,
    EnergyComponent& energy,
    DamageComponent& damage,
    Map const& map,
    Vector2I const& heroPosition,
    HealthComponent& heroHealth,
    GameCamera const& gameCamera)
{
    bool isActionMultiFrame{false};

    //* Instant action: attack
    if (Vector2Length(
            Vector2Subtract(
                position.tilePosition(),
                heroPosition))
        == 1)
    {
        //* Attack
        snx::Logger::log("Hero takes ");

        DamageSystem::attack(
            damage,
            heroHealth);

        energy.consume();
    }
    //* Check path
    else
    {
        std::vector<Vector2I> path{PathfinderSystem::findPath(
            map,
            position.tilePosition(),
            heroPosition,
            gameCamera,
            false,
            ai.scanRange)};

        size_t pathSize{path.size()};

        //* Path is valid: has at least 2 entries (start and target) -> Multi-frame action: move
        if (pathSize > 2
            && !CollisionSystem::checkCollision(
                map.tiles,
                map.enemies,
                path.rbegin()[1],
                heroPosition))
        {
            //* Prepare multi-frame action
            MovementSystem::prepareByFromTo(
                movement,
                transform,
                position.tilePosition(),
                path.rbegin()[1]);

            energy.consume();

            isActionMultiFrame = true;
        }
        //* Path is empty -> Instant action: wait
        else
        {
            //* Wait
            energy.consume();
        }
    }
    // return false;
    return isActionMultiFrame;
}
