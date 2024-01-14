#ifndef MAP_H_20240110231207
#define MAP_H_20240110231207

#include "ContiguousMap.h"
#include "raylib.h"
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

namespace snd
{
    typedef enum
    {
        FLOOR,
        WALL,
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

    private:
        ContiguousMap<Position, TileType> tiles_;
    };
}

#endif