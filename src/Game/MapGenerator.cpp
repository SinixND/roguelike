#include "MapGenerator.h"

#include "Map.h"
#include <raylib.h>
#include <unordered_set>

struct Area
{
    Vector2 cornerTopLeft;
    Vector2 cornerBottomRight;
};

Map MapGenerator::generateMap(int level)
{
    if (level == 0)
    {
        return getStartRoom();
    }

    /*TEMPORARY*/ return getStartRoom();
}

void MapGenerator::addRoom(Map& map, const Area& room)
{
    for (auto x{room.cornerTopLeft.x}; x <= room.cornerBottomRight.x; ++x)
    {
        for (auto y{room.cornerTopLeft.y}; y <= room.cornerBottomRight.y; ++y)
        {
            if ((x == room.cornerTopLeft.x) || (x == (room.cornerBottomRight.y)) || (y == room.cornerTopLeft.y) || (y == (room.cornerBottomRight.y)))
            {
                map.setTile({x, y}, WALL_TILE);
                continue;
            }

            map.setTile({x, y}, FLOOR_TILE);
        }
    }
}

void MapGenerator::set(Map& map, const Area& area, TileType tileType)
{
    for (auto x{area.cornerTopLeft.x}; x <= area.cornerBottomRight.x; ++x)
    {
        for (auto y{area.cornerTopLeft.y}; y <= area.cornerBottomRight.y; ++y)
        {
            map.setTile({x, y}, tileType);
        };
    }
}

Map MapGenerator::getStartRoom()
{
    Map map;
    addRoom(map, Area{{-7, -7}, {7, 7}});
    set(map, Area{{-1, -1}, {1, -1}}, WALL_TILE);
    set(map, Area{{-1, -7}, {1, -7}}, FLOOR_TILE);
    return map;
}
