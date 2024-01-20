#include "MapGenerator.h"

#include "Map.h"
#include <raylib.h>
#include <unordered_set>

struct Room
{
    Vector2 position;
    Vector2 dimensions;
};

Map MapGenerator::generateMap(int level)
{
    if (level == 0)
    {
        return getStartRoom();
    }

    /*TEMPORARY*/ return getStartRoom();
}

void MapGenerator::addRoom(Map& map, const Room& room)
{
    Vector2 roomCorner{
        (room.position.x + room.dimensions.x),
        (room.position.y + room.dimensions.y)};

    for (auto x{room.position.x}; x < roomCorner.x; ++x)
    {
        for (auto y{room.position.y}; y < roomCorner.y; ++y)
        {
            if ((x == room.position.x) || (x == (roomCorner.y - 1)) || (y == room.position.y) || (y == (roomCorner.y - 1)))
            {
                map.setTile({x, y}, WALL_TILE);
                continue;
            }

            map.setTile({x, y}, FLOOR_TILE);
        }
    }
}

Map MapGenerator::getStartRoom()
{
    Map map;
    addRoom(map, Room{{-7, -7}, {15, 15}});
    return map;
}
