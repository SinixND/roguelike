#ifndef IG20240308002046
#define IG20240308002046

#include "Unit.h"
#include <raylib.h>

namespace CameraControl
{
    void edgePan(Camera2D& camera, Vector2 cursorWorldPosition, bool isMouseControlled);
    void centerOnHero(Camera2D& camera, Unit& unit);
}

#endif
