#ifndef IG20240308002046
#define IG20240308002046

#include "Unit.h"
#include <raylib.h>

namespace CameraControl
{
    void edgePan(const Vector2& cursorWorldPosition, bool mouseActive);
    void centerOnHero(Camera2D& camera, Unit& unit);
}

#endif
