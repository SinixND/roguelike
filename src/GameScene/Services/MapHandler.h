#ifndef IG20240128151409
#define IG20240128151409

#include "Graphic.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

class World;

namespace MapHandler
{
    TileMap createNewMap(size_t level);

    void addTiles(
        TileMap& tileMap,
        const Area& area,
        Graphic graphic,
        bool isSolid,
        bool blocksVision);

    void addRoom(TileMap& tileMap, const Area& room);
    void createStartRoom(TileMap& tileMap);
    void createGridRooms(TileMap& tileMap, size_t level);
}

#endif