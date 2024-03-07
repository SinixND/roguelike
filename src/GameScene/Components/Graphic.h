#ifndef _20240223182955
#define _20240223182955

#include "LayerID.h"
#include "RenderID.h"

struct Graphic
{
    RenderID renderID;
    LayerID layerID;

    Graphic(
        RenderID renderID,
        LayerID layerID)
        : renderID(renderID)
        , layerID(layerID){};
};

#endif