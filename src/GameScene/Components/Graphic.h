#ifndef IG20240223182955
#define IG20240223182955

#include "LayerID.h"
#include "RenderID.h"

class GraphicComponent
{
public:
    auto renderID() const -> RenderID const& { return renderID_; }
    void setRenderID(RenderID const& renderID) { renderID_ = renderID; }

    auto layerID() const -> LayerID const& { return layerID_; }
    void setLayerID(LayerID const& layerID) { layerID_ = layerID; }

    explicit GraphicComponent(
        RenderID renderID = RenderID::INVISIBLE,
        LayerID layerID = LayerID::UI)
        : renderID_(renderID)
        , layerID_(layerID)
    {
    }

private:
    RenderID renderID_{};
    LayerID layerID_{};
};

#endif