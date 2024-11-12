#include "AISystem.h"

#include "DamageComponent.h"
#include "DamageSystem.h"
#include "Enemies.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "PathfinderSystem.h"
#include <cstddef>
#include <vector>

bool AISystem::checkForAction(
    Enemies& enemies,
    Map const& map,
    Vector2I const& heroPosition,
    HealthComponent& heroHealth,
    GameCamera const& gameCamera)
{
    size_t idSize{enemies.ids.size()};

    static size_t enemiesChecked{0};

    while (enemiesChecked < idSize)
    {
        size_t enemyID{enemies.ids.values()[enemiesChecked]};

        if (!enemies.energies.at(enemyID).isReady())
        {
            //* Cant perform action
            ++enemiesChecked;
            continue;
        }

        AISystem::chooseAction(
            enemies.ais.at(enemyID),
            enemies.positions.at(enemyID),
            enemies.movements.at(enemyID),
            enemies.energies.at(enemyID),
            enemies.damages.at(enemyID),
            map,
            heroPosition,
            heroHealth,
            gameCamera);

        return false;
    }

    //* All enemies checked
    enemiesChecked = 0;
    return true;
}

void AISystem::chooseAction(
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
    std::vector<Vector2I> path{PathfinderSystem::findPath(
        map,
        position.tilePosition(),
        heroPosition,
        gameCamera,
        false,
        ai.scanRange())};

    size_t pathSize{path.size()};

    if (pathSize == 0)
    {
        //* Wait
        energy.consume();
    }

    //* Path is either empty or has at least 2 entries (start and target)
    else if (pathSize == 2)
    {
        //* Attack
        snx::Logger::log("Hero takes ");

        DamageSystem::attack(
            damage,
            heroHealth);

        energy.consume();
    }

    else if (pathSize > 2)
    {
        movement.trigger(
            position.tilePosition(),
            path.rbegin()[1]);
    }

    //* MovementComponent is not viable
    else
    {
        //* Wait
        energy.consume();
    }
}
