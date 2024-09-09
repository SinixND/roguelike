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
#include "TileData.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <cstddef>

Vector2I Enemies::getRandomPosition(Tiles& tiles)
{
    // bool isPositionValid{false};
    RectangleExI const& mapSize{tiles.mapSize()};

    // while (!isPositionValid)
    while (true)
    {
        Vector2I randomPosition{
            snx::RNG::random(
                static_cast<int>(mapSize.left() / TileData::TILE_SIZE),
                static_cast<int>(mapSize.right() / TileData::TILE_SIZE)),
            snx::RNG::random(
                static_cast<int>(mapSize.top() / TileData::TILE_SIZE),
                static_cast<int>(mapSize.bottom() / TileData::TILE_SIZE))};

        if (
            tiles.positions().contains(randomPosition)
            && !tiles.isSolid(randomPosition)
            // && !positions_.contains(randomPosition)
        )
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
    Tiles& tiles)
{
    ids_.insert(id);
    renderIDs_.insert(id, renderID);
    ais_.insert(id, ai);
    movements_.insert(id, movement);
    energies_.insert(id, energy);
    positions_.insert(id, Position{getRandomPosition(tiles)});
}

void Enemies::create(RenderID enemyID, Tiles& tiles)
{
    size_t newID{idManager_.requestId()};

    switch (enemyID)
    {
    case RenderID::goblin:
    {
        insert(
            newID,
            RenderID::goblin,
            AI{},
            Movement{EnemyData::GOBLIN_SPEED},
            Energy{
                EnemyData::GOBLIN_MAX_ENERGY,
                EnemyData::GOBLIN_REGEN_RATE},
            tiles);
    }
    break;

    default:
        break;
    }
}

void Enemies::init(int mapLevel, Tiles& tiles)
{
    while (static_cast<int>(renderIDs_.size()) < (mapLevel + 1))
    {
        create(RenderID::goblin, tiles);
    }
}

void Enemies::update() {}

snx::DenseMap<size_t, size_t>& Enemies::ids() { return ids_; }

snx::DenseMap<size_t, Movement>& Enemies::movements() { return movements_; }
Movement& Enemies::movement(size_t id) { return movements_[id]; }

snx::DenseMap<size_t, Energy>& Enemies::energies() { return energies_; }
Energy& Enemies::energy(size_t id) { return energies_[id]; }

snx::DenseMap<size_t, Position>& Enemies::positions() { return positions_; }
Position& Enemies::position(size_t id) { return positions_[id]; }

snx::DenseMap<size_t, RenderID>& Enemies::renderIDs() { return renderIDs_; }
RenderID Enemies::renderID(size_t id) { return renderIDs_[id]; }

snx::DenseMap<size_t, AI>& Enemies::ais() { return ais_; }
AI& Enemies::ai(size_t id) { return ais_[id]; }
