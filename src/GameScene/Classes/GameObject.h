#ifndef IG20240211172120
#define IG20240211172120

#include "Graphic.h"
#include "Transformation.h"
#include <raylib.h>

class GameObject
{
public:
    Transformation transform{};
    Graphic graphic{};

    explicit GameObject(Transformation transform, Graphic graphic)
        : transform(transform)
        , graphic(graphic)
    {
    }
};

#endif