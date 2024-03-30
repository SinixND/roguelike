#ifndef IG20240209181841
#define IG20240209181841

#include "Graphic.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Render
{
    void update(Vector2 position, Graphic graphic, VisibilityID visibilityID = VisibilityID::VISIBLE);
    RectangleEx getRenderRectangle();
}

#endif