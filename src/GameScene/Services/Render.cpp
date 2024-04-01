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
        static RectangleEx renderRectangle{mapPanel()};
        static RectangleEx extendedRenderRectangle{
          renderRectangle.left() - TILE_SIZE,
          renderRectangle.top() - TILE_SIZE,
          renderRectangle.width() + (2 * TILE_SIZE),
          renderRectangle.height() + (2 * TILE_SIZE)};

        if (IsWindowResized())
        {
            renderRectangle = mapPanel();
            extendedRenderRectangle = {
              renderRectangle.left() - TILE_SIZE,
              renderRectangle.top() - TILE_SIZE,
              renderRectangle.width() + (2 * TILE_SIZE),
              renderRectangle.height() + (2 * TILE_SIZE)};
        }

        // Return if pixel is out of render area
        if (!CheckCollisionPointRec(GetWorldToScreen2D(position, dtb::camera()), extendedRenderRectangle))
            return;

        // Get texture data
        Texture2D const* textureAtlas{dtb::getTextureAtlas()};
        Vector2 texturePosition{dtb::getTexturePosition(graphic.renderID())};
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
            tint = ColorBrightness(tint, 0.33);
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

        BeginScissorMode(renderRectangle.left(), renderRectangle.top(), renderRectangle.width(), renderRectangle.height());
        // Draw texture (using 0.5f pixel offset to get rid of texture bleeding)
        DrawTexturePro(
          *textureAtlas,
          Rectangle{
            texturePosition.x + 0.5f,
            texturePosition.y + 0.5f,
            TEXTURE_WIDTH - (2 * 0.5f),
            TEXTURE_WIDTH - (2 * 0.5f)},
          Rectangle{
            position.x,
            position.y,
            tileSize.x,
            tileSize.y},
          tileCenter,
          0,
          tint);
        EndScissorMode();
    }

    RectangleEx mapPanel()
    {
        return RectangleEx{
          0,
          STATUS_PANEL_HEIGHT,
          GetRenderWidth() - INFO_PANEL_WIDTH,
          GetRenderHeight() - LOG_PANEL_HEIGHT - STATUS_PANEL_HEIGHT};
    }
}
