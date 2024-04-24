#ifndef IG20240211172120
#define IG20240211172120

#include "Graphic.h"
#include "Position.h"
#include <raylib.h>

class GameObject
{
public:
    PositionComponent positionComponent{};
    GraphicComponent graphicComponent{};

    explicit GameObject(
        PositionComponent positionComponent,
        GraphicComponent graphicComponent)
        : positionComponent(positionComponent)
        , graphicComponent(graphicComponent)
    {
    }
};

#endif