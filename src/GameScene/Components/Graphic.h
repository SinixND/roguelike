#ifndef IG20240223182955
#define IG20240223182955

#include "LayerID.h"
#include "RenderID.h"

class Graphic
{
public:
    RenderID const& renderID() const { return renderID_; };
    void setRenderID(RenderID const& renderID) { renderID_ = renderID; };

    LayerID const& layerID() const { return layerID_; };
    void setLayerID(LayerID const& layerID) { layerID_ = layerID; };

    Graphic(
        RenderID renderID,
        LayerID layerID)
        : renderID_(renderID)
        , layerID_(layerID){};

private:
    RenderID renderID_;
    LayerID layerID_;
};

#endif