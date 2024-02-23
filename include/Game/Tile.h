#ifndef _20240215012942
#define _20240215012942

#include "Graphic.h"
#include "raylibEx.h"

struct Tile
{
    Vector2Int position;
    Graphic graphic;
    bool isSolid;
};
#endif