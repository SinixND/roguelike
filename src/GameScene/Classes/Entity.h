#ifndef IG20240211172120
#define IG20240211172120

#include "Graphic.h"
#include "Transformation.h"
#include <raylib.h>

class Entity
{
public:
    Transformation transform{};
    Graphic graphic{};

    Entity(Transformation transform, Graphic graphic)
        : transform(transform)
        , graphic(graphic)
    {
    }
};

#endif