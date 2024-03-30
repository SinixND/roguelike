#ifndef IG20240128151409
#define IG20240128151409

#include "Graphic.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

class World;

namespace MapHandler
{
    TileMap createNewMap(int level);

    void addTiles(
      TileMap& tileMap,
      RectangleExI const& area,
      Graphic graphic,
      bool isSolid,
      bool blocksVision);

    void addRoom(TileMap& tileMap, RectangleExI const& room);
    void createStartRoom(TileMap& tileMap);
    void createGridRooms(TileMap& tileMap, int level);
}

#endif