#ifndef _20240215012942
#define _20240215012942

#include "Graphic.h"
#include "Position.h"

struct Tile
{
    Position position;
    Graphic graphic;
    bool isSolid;
};
#endif