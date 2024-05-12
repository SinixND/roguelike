#ifndef IG20240128151409
#define IG20240128151409

#include "MapSize.h"
#include "TileMap.h"

namespace MapGenerator
{
    auto createStartRoom(MapSize& mapSize) -> TileMap;
    auto createRandomMap(int level, MapSize& mapSize) -> TileMap;
}

#endif
