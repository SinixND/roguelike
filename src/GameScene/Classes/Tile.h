#ifndef IG20240215012942
#define IG20240215012942

#include "Graphic.h"
#include "Position.h"
#include "VisibilityID.h"
#include <string>

class Tile
{
public:
    PositionComponent positionComponent{};
    GraphicComponent graphicComponent{};

public:
    std::string const& tag() const { return tag_; }

    VisibilityID visibilityID() const { return visibilityID_; }
    void setVisibilityID(VisibilityID visibilityID) { visibilityID_ = visibilityID; }

    bool isSolid() const { return isSolid_; }

    bool blocksVision() const { return blocksVision_; }

    explicit Tile(
        std::string const& tag,
        PositionComponent const& positionComponent,
        GraphicComponent const& graphicComponent,
        VisibilityID visibilityID,
        bool isSolid,
        bool blocksVision)
        : positionComponent(positionComponent)
        , graphicComponent(graphicComponent)
        , tag_(tag)
        , visibilityID_(visibilityID)
        , isSolid_(isSolid)
        , blocksVision_(blocksVision)
    {
    }

private:
    std::string tag_{};
    VisibilityID visibilityID_{};
    bool isSolid_{};
    bool blocksVision_{};
};

#endif