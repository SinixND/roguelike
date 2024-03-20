#ifndef IG20240215012942
#define IG20240215012942

#include "Entity.h"
#include "Graphic.h"
#include "Position.h"
#include "VisibilityID.h"

class Tile : public Entity
{
public:
    bool isSolid;
    bool blocksVision;
    VisibilityID visibilityID;

    // For map tiles
    Tile(Position const& position, Graphic const& graphic, bool isSolid, bool blocksVision)
        : Entity(position, graphic)
        , isSolid(isSolid)
        , blocksVision(blocksVision)
        , visibilityID(VisibilityID::UNSEEN){};

    // For overlay tiles
    Tile(Position const& position, Graphic const& graphic)
        : Entity(position, graphic)
        , isSolid(false)
        , visibilityID(VisibilityID::VISIBLE){};
};
#endif