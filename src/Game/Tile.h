#ifndef _20240215012942
#define _20240215012942

#include "Entity.h"
#include "VisibilityID.h"

struct Tile : public Entity
{
    bool isSolid;
    bool blocksVision;
    VisibilityID visibilityID;

    // For map tiles
    Tile(const Position& position, const Graphic& graphic, bool isSolid, bool blocksVision)
        : Entity(position, graphic)
        , isSolid(isSolid)
        , blocksVision(blocksVision)
        , visibilityID(VisibilityID::unseen){};

    // For overlay tiles
    Tile(const Position& position, const Graphic& graphic)
        : Entity(position, graphic)
        , isSolid(false)
        , visibilityID(VisibilityID::visible){};
};
#endif