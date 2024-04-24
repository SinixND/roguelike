#ifndef IG20240128151409
#define IG20240128151409

#include "Graphic.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <string>

namespace MapGenerator
{
    auto createStartRoom() -> TileMap;
    auto createRandomMap(int level) -> TileMap;

    void addTiles(
        TileMap& tileMap,
        std::string tag,
        RectangleExI const& area,
        GraphicComponent graphic,
        bool isSolid,
        bool blocksVision);

    void addRoom(TileMap& tileMap, RectangleExI const& room);
    void createGridRooms(TileMap& tileMap, int level);
}

#endif