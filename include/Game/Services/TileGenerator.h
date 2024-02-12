#ifndef _20240128151409
#define _20240128151409

#include "RenderId.h"
#include "TileMap.h"
#include "raylib.h"
#include <cstddef>

class TileGenerator
{
public:
    TileMap createNewMap(size_t level);

private:
    void updateTiles(TileMap& tileMap, const Box& area, RenderId renderId, LayerId layerId, bool isSolid);
    void addRoom(TileMap& tileMap, const Box& room);
    void addStartRoom(TileMap& tileMap);
};

#endif