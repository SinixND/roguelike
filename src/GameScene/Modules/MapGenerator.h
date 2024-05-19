#ifndef IG20240128151409
#define IG20240128151409

#include "MapSize.h"
#include "TileMap.h"

namespace MapGenerator
{
    TileMap createStartRoom(MapSize& mapSize);
    TileMap createRandomMap(int level, MapSize& mapSize);
}

#endif
