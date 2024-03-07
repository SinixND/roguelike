#include "Render.h"

#include "Constants.h"
#include "Graphic.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "TileTransformation.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace {
    Rectangle getRenderArea();
}

void render(const Vector2& position, Graphic& graphic, VisibilityID visibilityID)
{
    static Rectangle RENDER_AREA{getRenderArea()};
    if (IsWindowResized())
        RENDER_AREA = getRenderArea();

    // Return if pixel is out of render area
    if (!CheckCollisionPointRec(GetWorldToScreen2D(position, dtb::camera()), RENDER_AREA))
        return;

    // Get texture data
    Texture2D* texture{dtb::getTexture(graphic.renderID)};
    Vector2 tileSize{TILE_DIMENSIONS};
    Vector2 tileCenter{Vector2Scale(tileSize, 0.5f)};

    // Consider visibility
    Color tint{WHITE};
    switch (visibilityID)
    {
    default:
    case VisibilityID::visible:
        tint = ColorBrightness(tint, 1);
        if (dtb::debugMode())
            tint = GREEN;
        break;

    case VisibilityID::seen:
        tint = BLACK;
        tint = ColorBrightness(tint, 0.5);
        if (dtb::debugMode())
        {
            tint = ColorBrightness(tint, 1);
            tint = BLUE;
        }
        break;

    case VisibilityID::unseen:
        tint = BLACK;
        tint = ColorBrightness(tint, 0.0);
        if (dtb::debugMode())
        {
            tint = ColorBrightness(tint, 0.75);
            tint = RED;
        }
        break;
    }

    // Draw texture
    DrawTexturePro(
        *texture,
        Rectangle{
            0,
            0,
            float(texture->width),
            float(texture->height)},
        Rectangle{
            position.x,
            position.y,
            tileSize.x,
            tileSize.y},
        tileCenter,
        0,
        tint);
}

namespace {
    Rectangle getRenderArea()
    {
        return Rectangle{
            -MAP_RENDER_AREA_MARGIN + LEFT_MAP_RENDER_OFFSET,
            -MAP_RENDER_AREA_MARGIN + TOP_MAP_RENDER_OFFSET,
            GetRenderWidth() - LEFT_MAP_RENDER_OFFSET - RIGHT_MAP_RENDER_OFFSET + 2 * MAP_RENDER_AREA_MARGIN,
            GetRenderHeight() - TOP_MAP_RENDER_OFFSET - BOTTOM_MAP_RENDER_OFFSET + 2 * MAP_RENDER_AREA_MARGIN};
    }
}
