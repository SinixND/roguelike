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
    auto tag() const -> std::string const& { return tag_; }

    auto visibilityID() const -> VisibilityID { return visibilityID_; }
    void setVisibilityID(VisibilityID visibilityID) { visibilityID_ = visibilityID; }

    auto isSolid() const -> bool { return isSolid_; }
    void setIsSolid(bool state) { isSolid_ = state; }

    auto blocksVision() const -> bool { return blocksVision_; }
    void setBlocksVision(bool state) { blocksVision_ = state; }

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