#include "AISystem.h"

#include "Enemies.h"
#include "PathfinderSystem.h"

#include <cstddef>

bool AISystem::checkForAction(
    Enemies& enemies,
    Map const& map,
    Vector2I const& heroPosition,
    GameCamera const& gameCamera)
{
    size_t idSize{enemies.ids.size()};

    static size_t enemiesChecked{0};

    while (enemiesChecked < idSize)
    {
        size_t enemyId{enemies.ids.values()[enemiesChecked]};

        if (!enemies.energies.at(enemyId).isReady())
        {
            //* Cant perform action
            ++enemiesChecked;
            continue;
        }

        AISystem::chooseAction(
                enemies.ais.at(enemyId),
                enemies.positions.at(enemyId),
                enemies.movements.at(enemyId),
                enemies.energies.at(enemyId),
                map, 
                heroPosition, 
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
    Map const& map,
    Vector2I const& heroPosition,
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
        //* Perform waiting action until attack is implemented
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

