#ifndef IG20240128151409
#define IG20240128151409

#include "MapSize.h"
#include "TileMap.h"

namespace MapGenerator
{
    auto createStartRoom(MapSize const& mapSize) -> TileMap;
    auto createRandomMap(int level, MapSize const& mapSize) -> TileMap;
}

#endif
