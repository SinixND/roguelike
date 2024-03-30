#ifndef IG20240215012942
#define IG20240215012942

#include "Entity.h"
#include "Graphic.h"
#include "Transformation.h"
#include "VisibilityID.h"

class Tile : public Entity
{
public:
    bool isSolid() const { return isSolid_; }
    void setIsSolid(bool isSolid) { isSolid_ = isSolid; }

    bool blocksVision() const { return blocksVision_; }
    void setBlocksVision(bool blocksVision) { blocksVision_ = blocksVision; }

    VisibilityID visibilityID() const { return visibilityID_; }
    void setVisibilityID(VisibilityID visibilityID)
    {
        visibilityID_ = visibilityID;
    }

    Tile(
      Transformation const& position,
      Graphic const& graphic,
      VisibilityID visibility,
      bool isSolid,
      bool blocksVision)
        : Entity(position, graphic)
        , visibilityID_(visibility)
        , isSolid_(isSolid)
        , blocksVision_(blocksVision)
    {
    }

    Tile(
      Transformation const& position,
      Graphic const& graphic,
      VisibilityID visibility)
        : Entity(position, graphic)
        , visibilityID_(visibility)
        , isSolid_(false)
    {
    }

private:
    VisibilityID visibilityID_;
    bool isSolid_;
    bool blocksVision_;
};

#endif