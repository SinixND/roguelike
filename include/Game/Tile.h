#ifndef _20240215012942
#define _20240215012942

#include "IEntity.h"
#include "Position.h"
#include "RenderData.h"

class Tile : public IEntity
{
public:
    Position position_{};
    RenderData renderData_{};
    bool isSolid_{};

    Tile(Position position, RenderData renderData, bool isSolid)
        : position_(position)
        , renderData_(renderData)
        , isSolid_(isSolid){};
};

#endif