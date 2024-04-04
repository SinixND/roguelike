#include "Render.h"

#include "Constants.h"
#include "Graphic.h"
#include "Panel.h"
#include "RuntimeDatabase.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace Render
{
    void update(Vector2 position, Graphic graphic, VisibilityID visibilityID)
    {
        static RectangleEx mapPanel{Panel::panelMap()};
        static RectangleEx extendedMapPanel{mapPanel};
        extendedMapPanel
          .offsetLeft(-TILE_SIZE)
          .offsetTop(-TILE_SIZE)
          .offsetRight(TILE_SIZE)
          .offsetBottom(TILE_SIZE);

        if (IsWindowResized())
        {
            mapPanel = Panel::panelMap();
            extendedMapPanel = mapPanel;
            extendedMapPanel
              .offsetLeft(-TILE_SIZE)
              .offsetTop(-TILE_SIZE)
              .offsetRight(TILE_SIZE)
              .offsetBottom(TILE_SIZE);
        }

        // Return if pixel is out of render area
        if (!CheckCollisionPointRec(GetWorldToScreen2D(position, dtb::camera()), extendedMapPanel))
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

        BeginScissorMode(mapPanel.left(), mapPanel.top(), mapPanel.width(), mapPanel.height());
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
}
