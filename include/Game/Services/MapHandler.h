#ifndef _20240128151409
#define _20240128151409

#include "RenderId.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

TileMap createNewMap(size_t level);

namespace MapHandler
{

    void updateTiles(TileMap& tileMap, const Area& area, RenderId renderId, bool isSolid);
    void addRoom(TileMap& tileMap, const Area& room);
    void addStartRoom(TileMap& tileMap);
}

#endif