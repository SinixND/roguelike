#ifndef MAPGENERATOR_H_20240110224943
#define MAPGENERATOR_H_20240110224943

#include "Components.h"
#include "Map.h"
#include "raylib.h"
#include <unordered_set>
#include <utility>

namespace snd
{
    struct Room
    {
        Vector2 position;
        Vector2 dimensions;
    };

    class MapGenerator
    {
    public:
        Map generateMap(int level)
        {
            if (level == 0)
            {
                return getStartRoom();
            }

            /*TEMPORARY*/ return getStartRoom();

            // create random adjacent room positions
            //* int maxRooms{9 + level};

            //* for (int i{0}; i < maxRooms; ++i)
            //* {
            //* }

            //* int roomSize{15};

            //* /*
            //* X X X X X X . . . X X X X X X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* . . . . . . . . . . . . . . .
            //* . . . . . . . S . . . . . . .
            //* . . . . . . . . . . . . . . .
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X . . . . . . . . . . . . . X
            //* X X X X X X . . . X X X X X X
            //* */

            //* return map_;
        };

    private:
        Map map_;
        std::unordered_set<Position> roomPositions_{{0, 0}};

    private:
        void addRoom(Map& map, const Room& room)
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

        Map getStartRoom()
        {
            Map map;
            addRoom(map, Room{{-7, -7}, {15, 15}});
            return map;
        }
    };
}

#endif