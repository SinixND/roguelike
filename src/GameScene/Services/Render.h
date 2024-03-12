#ifndef _20240209181841
#define _20240209181841

#include "Graphic.h"
#include "VisibilityID.h"
#include <raylib.h>

namespace Render
{
    void update(const Vector2& position, Graphic& graphic, VisibilityID visibilityID = VisibilityID::visible);
    Rectangle getRenderArea();
}

#endif