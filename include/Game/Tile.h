#ifndef _20240215012942
#define _20240215012942

#include "Graphic.h"
#include "LayerID.h"
#include "RenderID.h"
#include "raylibEx.h"

struct Tile
{
    Vector2i position;
    Graphic graphic;
    bool isSolid;
};
#endif