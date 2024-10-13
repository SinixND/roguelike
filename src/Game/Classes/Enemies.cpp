#include "Enemies.h"

#include "AI.h"
#include "Collision.h"
#include "Debugger.h"
#include "DenseMap.h"
#include "EnemyData.h"
#include "Energy.h"
#include "GameCamera.h"
#include "IdManager.h"
#include "Map.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
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

        if (
            tiles.positions().contains(randomPosition)
            && !(tiles.visibilityID(randomPosition) == VisibilityID::visible)
            && !tiles.isSolid(randomPosition)
            && !positions_.contains(Position{randomPosition}))
        {
            return randomPosition;
        }
    }
}

void Enemies::insert(
    size_t id,
    RenderID renderID,
    Movement const& movement,
    Energy const& energy,
    int scanRange,
    Vector2I const& tilePosition)
{
    ids_.push_back(id);
    renderIDs_.insert(id, renderID);
    movements_.insert(id, movement);
    energies_.insert(id, energy);
    positions_.insert(id, Position{tilePosition});
    ais_.insert(id, AI{scanRange});
}

void Enemies::create(
    Map const& map,
    RenderID enemyID,
    Vector2I tilePosition)
{
    // Allow creating enemy at specified position except heroPosition
    if (tilePosition == Vector2I{0, 0})
    {
        tilePosition = getRandomPosition(map.tiles_);
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
                Movement{20 * EnemyData::GOBLIN_BASE_AGILITY},
                Energy{EnemyData::GOBLIN_BASE_AGILITY},
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
        if (energy(enemyId).regenerate())
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

    static size_t i{0};

    while (i < idSize)
    {
        size_t enemyId{ids_[i]};

        if (!energy(enemyId).isReady())
        {
            // Cant perform action
            ++i;
            continue;
        }

        Vector2I enemyPosition{position(enemyId).tilePosition()};

        switch (renderID(enemyId))
        {
            case RenderID::goblin:
            {
                std::vector<Vector2I> path{Pathfinder::findPath(
                    map,
                    enemyPosition,
                    heroPosition,
                    gameCamera,
                    false,
                    ai(enemyId).scanRange())};

                size_t pathSize{path.size()};

                if (pathSize == 0)
                {
                    // Wait
                    energy(enemyId).consume();
                }

                // Path is either empty or has at least 2 entries (start and target)
                else if (pathSize == 2)
                {
                    // Attack
                    // Perform waiting action until attack is implemented
                    energy(enemyId).consume();
                }

                else if (
                    pathSize > 2
                    && !Collision::checkCollision(map, path.rbegin()[1], heroPosition))
                {
                    movement(enemyId).trigger(
                        Vector2Subtract(
                            path.rbegin()[1],
                            enemyPosition));
                }

                // Movement is not viable
                else
                {
                    // Wait
                    energy(enemyId).consume();
                }

                break;
            }

            default:
                break;
        }

        // Return after an action has been triggered
        return false;
    }

    // All enemies checked
    i = 0;
    return true;
}

void Enemies::update(
    Map const& map,
    Position const& heroPosition)
{
    // TODO: consider "day" of last update in loop to handle dying enemies!
    for (size_t const& enemyId : ids_)
    {
        movement(enemyId).update(
            position(enemyId),
            energy(enemyId),
            map,
            heroPosition);
    }
}

snx::DenseMap<size_t, Movement> const& Enemies::movements() const
{
    return movements_;
}

Movement const& Enemies::movement(size_t id) const
{
    return movements_.at(id);
}

Movement& Enemies::movement(size_t id)
{
    return movements_.at(id);
}

snx::DenseMap<size_t, Energy> const& Enemies::energies() const
{
    return energies_;
}

Energy const& Enemies::energy(size_t id) const
{
    return energies_.at(id);
}

Energy& Enemies::energy(size_t id)
{
    return energies_.at(id);
}

snx::DenseMap<size_t, Position> const& Enemies::positions() const
{
    return positions_;
}

Position const& Enemies::position(size_t id) const
{
    return positions_.at(id);
}

Position& Enemies::position(size_t id)
{
    return positions_.at(id);
}

snx::DenseMap<size_t, RenderID> const& Enemies::renderIDs() const
{
    return renderIDs_;
}

RenderID Enemies::renderID(size_t id) const
{
    return renderIDs_.at(id);
}

snx::DenseMap<size_t, AI> const& Enemies::ais() const
{
    return ais_;
}

AI const& Enemies::ai(size_t id) const
{
    return ais_.at(id);
}

AI& Enemies::ai(size_t id)
{
    return ais_.at(id);
}

std::vector<size_t> const& Enemies::ids() const
{
    return ids_;
}
