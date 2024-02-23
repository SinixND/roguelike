#ifndef _20240223182955
#define _20240223182955

#include "LayerID.h"
#include "RenderID.h"
#include "raylibEx.h"

struct Graphic
{
    Vector2Int* position;

    RenderID renderID;
    LayerID layerID;
};

#endif