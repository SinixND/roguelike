#ifndef IG20240209181841
#define IG20240209181841

#include "DebugMode.h"
#include "Graphic.h"
#include "Textures.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Render
{
    void update(
        Vector2 position,
        GraphicComponent graphic,
        Camera2D const& camera,
        Textures& gameTextures,
        DebugMode const& debugMode,
        VisibilityID visibilityID = VisibilityID::VISIBLE);

    RectangleEx panelMap();
}

#endif