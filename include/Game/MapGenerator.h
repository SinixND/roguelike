#ifndef MAPGENERATOR_H_20240110224943
#define MAPGENERATOR_H_20240110224943

#include "Map.h"
#include <raylib.h>
#include <unordered_set>

namespace snd
{
    struct Room;

    class MapGenerator
    {
    public:
        Map generateMap(int level);

    private:
        Map map_{};
        std::unordered_set<Position> roomPositions_{{0, 0}};

    private:
        void addRoom(Map& map, const Room& room);

        Map getStartRoom();
    };
}

#endif