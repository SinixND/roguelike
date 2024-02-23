#ifndef _20240215012942
#define _20240215012942

#include "RenderID.h"
#include "raylibEx.h"

struct Tile
{
    Vector2Int position;
    RenderID renderID;
    bool isSolid;
};
#endif