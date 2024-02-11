#ifndef _20240211230403
#define _20240211230403

#include "Entity.h"
#include "Range.h"

class Unit : public Entity
{
public:
    Range moveRange_{};

    Unit(Range moveRange)
        : moveRange_(moveRange)
    {
    }
};

#endif