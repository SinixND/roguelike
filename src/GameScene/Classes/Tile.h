#ifndef IG20240215012942
#define IG20240215012942

#include "Entity.h"
#include "Graphic.h"
#include "Transformation.h"
#include "VisibilityID.h"

class Tile : public Entity
{
public:
    bool isSolid() const { return isSolid_; };
    void setIsSolid(bool isSolid) { isSolid_ = isSolid; };

    bool blocksVision() const { return blocksVision_; };
    void setBlocksVision(bool blocksVision) { blocksVision_ = blocksVision; };

    VisibilityID visibilityID() const { return visibilityID_; };
    void setVisibilityID(VisibilityID visibilityID) { visibilityID_ = visibilityID; };

    Tile(Transformation const& position, Graphic const& graphic, bool isSolid, bool blocksVision)
        : Entity(position, graphic)
        , isSolid_(isSolid)
        , blocksVision_(blocksVision)
        , visibilityID_(VisibilityID::UNSEEN){};

    // For overlay tiles
    Tile(Transformation const& position, Graphic const& graphic)
        : Entity(position, graphic)
        , isSolid_(false)
        , visibilityID_(VisibilityID::VISIBLE){};

private:
    bool isSolid_;
    bool blocksVision_;
    VisibilityID visibilityID_;
};
#endif