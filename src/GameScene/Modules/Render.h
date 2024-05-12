#ifndef IG20240209181841
#define IG20240209181841

#include "Graphic.h"
#include "Textures.h"
#include "VisibilityID.h"
#include <raylib.h>

namespace Render
{
    void update(
        Vector2 position,
        GraphicComponent graphic,
        Camera2D const& camera,
        Textures& gameTextures,
        bool cheatMode,
        VisibilityID visibilityID = VisibilityID::VISIBLE);
}

#endif