#include "Enemies.h"

#include "AI.h"
#include "DenseMap.h"
#include "EnemyData.h"
#include "Energy.h"
#include "IdManager.h"
#include "Movement.h"
#include "Position.h"
#include "RNG.h"
#include "RenderID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <cstddef>

Vector2I Enemies::getRandomPosition(Tiles const& tiles)
{
    // bool isPositionValid{false};
    RectangleExI const& mapSize{tiles.mapSize()};

    Vector2I randomPosition{};

    // while (!isPositionValid)
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
            && !tiles.isSolid(randomPosition)
            && !ids_.contains(randomPosition))
        {
            // isPositionValid = true;
            return randomPosition;
        }
    }

    // return randomPosition;
}

void Enemies::insert(
    size_t id,
    RenderID renderID,
    AI const& ai,
    Movement const& movement,
    Energy const& energy,
    Vector2I const& tilePosition)
{
    renderIDs_.insert(id, renderID);
    ais_.insert(id, ai);
    movements_.insert(id, movement);
    energies_.insert(id, energy);
    positions_.insert(id, Position{tilePosition});
    ids_.insert(tilePosition, id);
}

void Enemies::create(
    Tiles const& tiles,
    RenderID enemyID,
    Vector2I tilePosition)
{
    // Allow creating enemy at specified position except {0, 0}
    // TODO: Rather check for visiblity
    if (tilePosition == Vector2I{0, 0})
    {
        tilePosition = getRandomPosition(tiles);
    }

    size_t newID{idManager_.requestId()};

    switch (enemyID)
    {
    case RenderID::goblin:
    {
        insert(
            newID,
            RenderID::goblin,
            AI{},
            Movement{EnemyData::GOBLIN_BASE_AGILITY},
            Energy{EnemyData::GOBLIN_BASE_AGILITY},
            tilePosition);
    }

    break;

    default:
        break;
    }
}

void Enemies::init(int mapLevel, Tiles const& tiles)
{
    while (static_cast<int>(renderIDs_.size()) < ((mapLevel + 1) * 5))
    {
        create(tiles, RenderID::goblin);
    }
}

bool Enemies::update()
{
    // for(auto& energy : energies_)
    // {
    //     if(energy.regenerate())
    //     {

    //     }

    // }
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
