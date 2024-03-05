#ifndef _20240128151409
#define _20240128151409

#include "RenderID.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

TileMap createNewMap(size_t level);

namespace MapHandler
{

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