#include "Enemies.h"

#include "AI.h"
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
#include <string>

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
            && !ids_.contains(randomPosition))
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
    renderIDs_.insert(id, renderID);
    movements_.insert(id, movement);
    energies_.insert(id, energy);
    positions_.insert(id, Position{tilePosition});
    ids_.insert(tilePosition, id);
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
    case RenderID::goblin:
    {
        insert(
            newID,
            RenderID::goblin,
            Movement{EnemyData::GOBLIN_BASE_AGILITY},
            Energy{EnemyData::GOBLIN_BASE_AGILITY},
            EnemyData::GOBLIN_SCAN_RANGE,
            tilePosition);
    }

    break;

    default:
        break;
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

bool Enemies::update(
    Map const& map,
    Vector2I const& heroPosition,
    GameCamera const& gameCamera)
{
    static size_t enemy{0};

    size_t enemies{energies_.size()};

    while(enemy < enemies)
    {
        snx::debug::cliLog("Enemy #" + std::to_string(enemy) + "\n");
        if(!energy(enemy).regenerate())
        {
            continue;
        }

        switch(renderID(enemy))
        {
            case RenderID::goblin:
            {
                Pathfinder::findPath(
                    map, 
                    position(enemy).tilePosition(), 
                    heroPosition, 
                    gameCamera,
                    ai(enemy).scanRange());
                
                //if path = 0: wait
                //if path = 1: attack
                //if path > 1: move
                //break;
            }
            default:
            break;
        }

        ++enemy;
        static int round{0};
        snx::debug::cliLog("Round #" + std::to_string(round) + "\n");
    }

    if (enemy == enemies)
    { 
        enemy = 0;
    }

    return true;
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

snx::DenseMap<Vector2I, size_t> const& Enemies::ids() const
{
    return ids_;
}

size_t Enemies::id(Vector2I const& tilePosition) const
{
    return ids_.at(tilePosition);
}
