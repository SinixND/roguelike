#ifndef MAPGENERATOR_H_20240110224943
#define MAPGENERATOR_H_20240110224943

#include "Map.h"
#include <raylib.h>
#include <unordered_set>

struct Area;

class MapGenerator
{
public:
    Map generateMap(int level);

private:
    Map map_{};
    std::unordered_set<Position> roomPositions_{{0, 0}};

private:
    void addRoom(Map& map, const Area& room);
    void set(Map& map, const Area& area, TileType tileType);

    Map getStartRoom();
};

#endif