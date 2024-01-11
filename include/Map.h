#ifndef MAP_H_20240110231207
#define MAP_H_20240110231207

#include "Components.h"
#include "raylib.h"
#include <utility>
#include <vector>

namespace snd
{
    using Position = Vector2;

    typedef enum
    {
        FLOOR,
        WALL,
    } TileType;

    struct Map
    {
        std::vector<std::pair<Position, TileType>> tiles_;
    };
}

#endif