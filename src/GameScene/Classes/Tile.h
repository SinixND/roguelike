#ifndef IG20240215012942
#define IG20240215012942

#include "GameObject.h"
#include "Graphic.h"
#include "Transformation.h"
#include "VisibilityID.h"
#include <string>

class Tile : public GameObject
{
public:
    std::string const& tag() const { return tag_; }

    bool isSolid() const { return isSolid_; }
    void setIsSolid(bool state) { isSolid_ = state; }

    bool blocksVision() const { return blocksVision_; }
    void setBlocksVision(bool state) { blocksVision_ = state; }

    VisibilityID visibilityID() const { return visibilityID_; }
    void setVisibilityID(VisibilityID visibilityID)
    {
        visibilityID_ = visibilityID;
    }

    explicit Tile(
        std::string const& tag,
        Transformation const& transform,
        Graphic const& graphic,
        VisibilityID visibility,
        bool isSolid,
        bool blocksVision)
        : GameObject(transform, graphic)
        , tag_(tag)
        , visibilityID_(visibility)
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