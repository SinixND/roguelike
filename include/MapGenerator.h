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
            for (auto x{room.position.x}; x < room.dimensions.x; ++x)
            {
                for (auto y{room.position.y}; y < room.dimensions.y; ++y)
                {
                    if (((x == room.position.x) || (x == room.position.y)) && ((y == room.dimensions.x) || (y == room.dimensions.y)))
                    {
                        map.setTile({x, y}, WALL);
                        continue;
                    }

                    map.setTile({x, y}, FLOOR);
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