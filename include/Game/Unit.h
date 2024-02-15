#ifndef _20240211230403
#define _20240211230403

#include "IEntity.h"
#include "Position.h"
#include "Range.h"
#include "RenderData.h"

class Unit : public IEntity
{
public:
    Position position_{};
    RenderData renderData{};
    Range moveRange_{};

    Unit(Range moveRange)
        : moveRange_(moveRange)
    {
    }
};

#endif