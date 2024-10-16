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
#include <utility>
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
            tiles.getPositions().contains(randomPosition)
            && !(tiles.getVisibilityIDs().at(randomPosition) == VisibilityID::visible)
            && !tiles.getIsSolids().contains(randomPosition)
            && !positions_.contains(PositionComponent{randomPosition}))
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
    ids_.push_back(id);
    renderIDs_.insert(id, renderID);
    movements_.insert(id, movement);
    energies_.insert(id, energy);
    positions_.insert(id, PositionComponent{tilePosition});
    ais_.insert(id, AIComponent{scanRange});
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
    while (static_cast<int>(renderIDs_.size()) < ((mapLevel + 1) * 5))
    {
        create(
            map,
            RenderID::goblin);
    }
}

bool Enemies::regenerate()
{
    bool isEnemyReady{false};

    for (size_t const& enemyId : ids_)
    {
        if (energies_.at(enemyId).regenerate())
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
    size_t idSize{ids_.size()};

    static size_t enemiesChecked{0};

    while (enemiesChecked < idSize)
    {
        size_t enemyId{ids_[enemiesChecked]};

        if (!energies_.at(enemyId).isReady())
        {
            //* Cant perform action
            ++enemiesChecked;
            continue;
        }

        Vector2I enemyPosition{positions_.at(enemyId).tilePosition()};

        switch (renderIDs_.at(enemyId))
        {
            case RenderID::goblin:
            {
                std::vector<Vector2I> path{PathfinderSystem::findPath(
                    map,
                    enemyPosition,
                    heroPosition,
                    gameCamera,
                    false,
                    ais_.at(enemyId).scanRange())};

                size_t pathSize{path.size()};

                if (pathSize == 0)
                {
                    //* Wait
                    energies_.at(enemyId).consume();
                }

                //* Path is either empty or has at least 2 entries (start and target)
                else if (pathSize == 2)
                {
                    //* Attack
                    //* Perform waiting action until attack is implemented
                    energies_.at(enemyId).consume();
                }

                else if (pathSize > 2)
                {
                    movements_.at(enemyId).trigger(
                        Vector2Subtract(
                            path.rbegin()[1],
                            enemyPosition));
                }

                //* MovementComponent is not viable
                else
                {
                    //* Wait
                    energies_.at(enemyId).consume();
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
    for (size_t const& enemyId : ids_)
    {
        movements_.at(enemyId).update(
            positions_.at(enemyId),
            energies_.at(enemyId),
            map,
            heroPosition);
    }
}

std::vector<size_t> const& Enemies::getIds() const
{
    return ids_;
}

snx::DenseMap<size_t, AIComponent> const& Enemies::getAIs() const
{
    return ais_;
}

snx::DenseMap<size_t, AIComponent>& Enemies::getAIs()
{
    return const_cast<snx::DenseMap<size_t, AIComponent>&>(std::as_const(*this).getAIs());
}

snx::DenseMap<size_t, PositionComponent> const& Enemies::getPositions() const
{
    return positions_;
}

snx::DenseMap<size_t, PositionComponent>& Enemies::getPositions()
{
    return const_cast<snx::DenseMap<size_t, PositionComponent>&>(std::as_const(*this).getPositions());
}

snx::DenseMap<size_t, RenderID> const& Enemies::getRenderIDs() const
{
    return renderIDs_;
}

snx::DenseMap<size_t, RenderID>& Enemies::getRenderIDs()
{
    return const_cast<snx::DenseMap<size_t, RenderID>&>(std::as_const(*this).getRenderIDs());
}

snx::DenseMap<size_t, MovementComponent> const& Enemies::getMovements() const
{
    return movements_;
}

snx::DenseMap<size_t, MovementComponent>& Enemies::getMovements()
{
    return const_cast<snx::DenseMap<size_t, MovementComponent>&>(std::as_const(*this).getMovements());
}

snx::DenseMap<size_t, EnergyComponent> const& Enemies::gitEnergies() const
{
    return energies_;
}

snx::DenseMap<size_t, EnergyComponent>& Enemies::getEnergies()
{
    return const_cast<snx::DenseMap<size_t, EnergyComponent>&>(std::as_const(*this).gitEnergies());
}
