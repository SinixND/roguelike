#ifndef MAP_H_20240110231207
#define MAP_H_20240110231207

#include "ContiguousMap.h"
#include "EntityId.h"
#include <raylib.h>
#include <vector>

using Position = std::pair<int, int>;

// make position pair usable in unordered container
template <>
struct std::hash<Position>
{
    std::size_t operator()(const Position& position) const noexcept
    {
        return position.first + 10 * position.second;
    }
};

typedef enum
{
    FLOOR_TILE,
    WALL_TILE,
} TileType;

struct Map
{
    void setTile(Position position, TileType tile)
    {
        tiles_.insert(position, tile);
    }

    TileType getTile(Position position)
    {
        return *tiles_.get(position);
    }

    snd::ContiguousMap<Position, TileType>* getTiles() { return &tiles_; };

    snd::EntityId addEntity(snd::EntityId entityId)
    {
        entities_.push_back(entityId);
        return entityId;
    }

private:
    snd::ContiguousMap<Position, TileType> tiles_;
    std::vector<snd::EntityId> entities_;
};

#endif