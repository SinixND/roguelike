#ifndef IG20240128151409
#define IG20240128151409

#include "Graphic.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

namespace MapHandler
{
    TileMap createNewMap(size_t level);

    void createTiles(
        TileMap& tileMap,
        const Area& area,
        Graphic graphic,
        bool isSolid,
        bool blocksVision);

    void addRoom(TileMap& tileMap, const Area& room);
    void addStartRoom(TileMap& tileMap);
}

#endif