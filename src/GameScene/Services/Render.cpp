#include "Render.h"

#include "Constants.h"
#include "Graphic.h"
#include "RuntimeDatabase.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace Render
{
    void update(Vector2 position, Graphic graphic, VisibilityID visibilityID)
    {
        static RectangleEx renderRectangle{getRenderRectangle()};
        if (IsWindowResized())
            renderRectangle = getRenderRectangle();

        // Return if pixel is out of render area
        if (!CheckCollisionPointRec(GetWorldToScreen2D(position, dtb::camera()), renderRectangle))
            return;

        // Get texture data
        Texture2D const* texture{dtb::getTexture(graphic.renderID())};
        Vector2 tileSize{TILE_DIMENSIONS};
        Vector2 tileCenter{Vector2Scale(tileSize, 0.5f)};

        // Consider visibility
        Color tint{WHITE};

        switch (visibilityID)
        {
        default:
        case VisibilityID::VISIBLE:
            tint = ColorBrightness(tint, 1);
            break;

        case VisibilityID::SEEN:
            tint = BLACK;
            tint = ColorBrightness(tint, 0.67);
            break;

        case VisibilityID::UNSEEN:
            tint = BLACK;
            tint = ColorBrightness(tint, 0.0);

            if (dtb::debugMode())
            {
                tint = ColorBrightness(tint, 0.67);
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

    RectangleEx getRenderRectangle()
    {
        return RectangleEx{
          -MAP_RENDER_AREA_MARGIN + LEFT_MAP_RENDER_OFFSET,
          -MAP_RENDER_AREA_MARGIN + TOP_MAP_RENDER_OFFSET,
          GetRenderWidth() - LEFT_MAP_RENDER_OFFSET - RIGHT_MAP_RENDER_OFFSET + 2 * MAP_RENDER_AREA_MARGIN,
          GetRenderHeight() - TOP_MAP_RENDER_OFFSET - BOTTOM_MAP_RENDER_OFFSET + 2 * MAP_RENDER_AREA_MARGIN};
    }
}
