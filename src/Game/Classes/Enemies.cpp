#include "Enemies.h"

#include "AIComponent.h"
#include "DenseMap.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "GameCamera.h"
#include "IdManager.h"
#include "Map.h"
#include "MovementComponent.h"
#include "PathfinderSystem.h"
#include "PositionComponent.h"
#include "RNG.h"
#include "RenderID.h"
#include "Tiles.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

Vector2I Enemies::getRandomPosition(Tiles const& tiles)
{
    RectangleExI const& mapSize{tiles.mapSize()};

    Vector2I randomPosition{};

    while (true)
    {
        randomPosition.x = snx::RNG::random(
            mapSize.left(),
            mapSize.right());

        randomPosition.y = snx::RNG::random(
            mapSize.top(),
            mapSize.bottom());

        //* Check if random position is
        //* - on map
        //* - not visible
        //* - not solid
        //* - not occupied by other enemy
        if (
            tiles.positions.contains(randomPosition)
            && !(tiles.visibilityIDs.at(randomPosition) == VisibilityID::visible)
            && !tiles.isSolid(randomPosition)
            && !this->ids.contains(randomPosition))
        {
            return randomPosition;
        }
    }
}

void Enemies::insert(
    size_t id,
    RenderID renderID,
    MovementComponent const& movement,
    EnergyComponent const& energy,
    int scanRange,
    Vector2I const& tilePosition)
{
    ids.insert(tilePosition, id);
    positions.insert(id, PositionComponent{tilePosition});
    renderIDs.insert(id, renderID);
    movements.insert(id, movement);
    energies.insert(id, energy);
    ais.insert(id, AIComponent{scanRange});
}

void Enemies::create(
    Map const& map,
    RenderID enemyID,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position except heroPosition
    if (tilePosition == Vector2I{0, 0})
    {
        tilePosition = getRandomPosition(map.tiles);
    }

    size_t newID{idManager_.requestId()};

    switch (enemyID)
    {
        default:
        case RenderID::goblin:
        {
            insert(
                newID,
                RenderID::goblin,
                MovementComponent{20 * EnemyData::GOBLIN_BASE_AGILITY},
                EnergyComponent{EnemyData::GOBLIN_BASE_AGILITY},
                EnemyData::GOBLIN_SCAN_RANGE,
                tilePosition);

            break;
        }
    }
}

void Enemies::init(
    int mapLevel,
    Map const& map)
{
    while (static_cast<int>(renderIDs.size()) < ((mapLevel + 1) * 5))
    {
        create(
            map,
            RenderID::goblin);
    }
}

bool Enemies::regenerate()
{
    bool isEnemyReady{false};

    for (size_t const& enemyId : ids.values())
    {
        if (energies.at(enemyId).regenerate())
        {
            isEnemyReady = true;
        }
    }

    return isEnemyReady;
}

bool Enemies::checkForAction(
    Map const& map,
    Vector2I const& heroPosition,
    GameCamera const& gameCamera)
{
    size_t idSize{ids.size()};

    static size_t enemiesChecked{0};

    while (enemiesChecked < idSize)
    {
        size_t enemyId{ids.values()[enemiesChecked]};

        if (!energies.at(enemyId).isReady())
        {
            //* Cant perform action
            ++enemiesChecked;
            continue;
        }

        Vector2I enemyPosition{positions.at(enemyId).tilePosition()};

        switch (renderIDs.at(enemyId))
        {
            case RenderID::goblin:
            {
                std::vector<Vector2I> path{PathfinderSystem::findPath(
                    map,
                    enemyPosition,
                    heroPosition,
                    gameCamera,
                    false,
                    ais.at(enemyId).scanRange())};

                size_t pathSize{path.size()};

                if (pathSize == 0)
                {
                    //* Wait
                    energies.at(enemyId).consume();
                }

                //* Path is either empty or has at least 2 entries (start and target)
                else if (pathSize == 2)
                {
                    //* Attack
                    //* Perform waiting action until attack is implemented
                    energies.at(enemyId).consume();
                }

                else if (pathSize > 2)
                {
                    movements.at(enemyId).trigger(
                        Vector2Subtract(
                            path.rbegin()[1],
                            enemyPosition));
                }

                //* MovementComponent is not viable
                else
                {
                    //* Wait
                    energies.at(enemyId).consume();
                }

                break;
            }

            default:
                break;
        }

        //* Return after an action has been triggered
        return false;
    }

    //* All enemies checked
    enemiesChecked = 0;
    return true;
}

void Enemies::update(
    Map const& map,
    PositionComponent const& heroPosition)
{
    //* TODO: consider "day" of last update in loop to handle dying enemies!
    for (size_t const& enemyId : ids.values())
    {
        PositionComponent& position{positions.at(enemyId)};

        Vector2I oldPosition{position.tilePosition()};

        //* Update ids_ key if tilePosition changes
        if (
            movements.at(enemyId).update(
                position,
                energies.at(enemyId),
                map,
                heroPosition))
        {
            ids.changeKey(
                oldPosition,
                position.tilePosition());
        }
    }
}
