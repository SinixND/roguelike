#ifndef MAPGENERATOR_H_20240110224943
#define MAPGENERATOR_H_20240110224943

#include "Components.h"
#include "Map.h"
#include "raylib.h"
#include <unordered_set>
#include <utility>

using Position = std::pair<int, int>;
using Room = Vector4;

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
    class MapGenerator
    {
    public:
        Map generateMap(int level)
        {
            // create random adjacent room positions
            int maxRooms{9 + level};

            for (int i{0}; i < maxRooms; ++i)
            {
            }

            int roomSize{15};

            /*
            X X X X X X . . . X X X X X X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            . . . . . . . . . . . . . . .
            . . . . . . . S . . . . . . .
            . . . . . . . . . . . . . . .
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X . . . . . . . . . . . . . X
            X X X X X X . . . X X X X X X
            */

            return map_;
        };

    private:
        Map map_;
        std::unordered_set<Position> roomPositions_{{0, 0}};
    };
}

#endif