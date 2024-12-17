#include "AISystem.h"

#include "CollisionSystem.h"
#include "DamageComponent.h"
#include "DamageSystem.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
#include <cstddef>
#include <vector>

// bool AISystem::checkReadiness(
//     Enemies& enemies,
//     Map const& map,
//     Vector2I const& heroPosition,
//     HealthComponent& heroHealth,
//     GameCamera const& gameCamera)
// {
//     size_t idSize{enemies.ids.size()};
//
//     static size_t enemiesChecked{0};
//
//     while (enemiesChecked < idSize)
//     {
//         size_t enemyId{enemies.ids.values()[enemiesChecked]};
//
//         if (!enemies.energies.at(enemyId).isReady())
//         {
//             //* Cant perform action
//             ++enemiesChecked;
//             continue;
//         }
//
//         AISystem::takeAction(
//             enemies.ais.at(enemyId),
//             enemies.positions.at(enemyId),
//             enemies.movements.at(enemyId),
//             enemies.energies.at(enemyId),
//             enemies.damages.at(enemyId),
//             map,
//             heroPosition,
//             heroHealth,
//             gameCamera);
//
//         return false;
//     }
//
//     //* All enemies checked
//     enemiesChecked = 0;
//     return true;
// }

bool AISystem::takeAction(
    AIComponent const& ai,
    PositionComponent& position,
    MovementComponent& movement,
    EnergyComponent& energy,
    DamageComponent& damage,
    Map const& map,
    Vector2I const& heroPosition,
    HealthComponent& heroHealth,
    GameCamera const& gameCamera)
{
    //* Instant action
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

        return false;
    }

    //* Prepare multi-frame action
    std::vector<Vector2I> path{PathfinderSystem::findPath(
        map,
        position.tilePosition(),
        heroPosition,
        gameCamera,
        false,
        ai.scanRange)};

    size_t pathSize{path.size()};

    //* Path is either empty or has at least 2 entries (start and target)
    if (pathSize > 2
        && !CollisionSystem::checkCollision(
            map.tiles,
            map.enemies,
            // map.objects,
            Vector2Add(
                position.tilePosition(),
                path.rbegin()[1]),
            heroPosition))
    {
        MovementSystem::prepareByFromTo(
            movement,
            position,
            position.tilePosition(),
            path.rbegin()[1]);

        return true;
    }

    //* TransformComponent is not viable
    //* Wait
    energy.consume();

    return false;
}
