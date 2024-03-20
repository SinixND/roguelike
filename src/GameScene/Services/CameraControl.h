#ifndef IG20240308002046
#define IG20240308002046

#include "Unit.h"
#include <raylib.h>

namespace CameraControl
{
    void edgePan(Vector2 cursorWorldPosition, bool isMouseActive);
    void centerOnHero(Camera2D& camera, Unit& unit);
}

#endif
