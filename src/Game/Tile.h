#ifndef _20240215012942
#define _20240215012942

#include "Graphic.h"
#include "TilePosition.h"

struct Tile
{
    TilePosition tilePosition;
    Graphic graphic;
    bool isSolid;
};
#endif