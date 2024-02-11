#ifndef _20240128151409
#define _20240128151409

#include "Attributes/RenderId.h"
#include "TileMap.h"
#include "raylib.h"
#include <cstddef>

class MapGenerator
{
public:
    TileMap createNewMap(size_t level);

private:
    void updateTiles(TileMap& tileMap, const Box& area, RenderId tileId);
    void addRoom(TileMap& tileMap, const Box& room);
    void addStartRoom(TileMap& tileMap);
};

#endif