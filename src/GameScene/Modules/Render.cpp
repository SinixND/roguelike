#include "Render.h"

#include "Graphic.h"
#include "Panels.h"
#include "RuntimeDatabase.h"
#include "Textures.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace Render
{
    void update(Vector2 position, Graphic graphic, VisibilityID visibilityID)
    {
        static RectangleEx mapPanel{PanelMap::panel()};
        static RectangleEx extendedMapPanel{mapPanel};
        extendedMapPanel
            .offsetLeft(-Textures::TILE_SIZE)
            .offsetTop(-Textures::TILE_SIZE)
            .offsetRight(Textures::TILE_SIZE)
            .offsetBottom(Textures::TILE_SIZE);

        if (IsWindowResized())
        {
            mapPanel = PanelMap::panel();
            extendedMapPanel = mapPanel;
            extendedMapPanel
                .offsetLeft(-Textures::TILE_SIZE)
                .offsetTop(-Textures::TILE_SIZE)
                .offsetRight(Textures::TILE_SIZE)
                .offsetBottom(Textures::TILE_SIZE);
        }

        // Return if pixel is out of render area
        if (!CheckCollisionPointRec(GetWorldToScreen2D(position, dtb::camera()), extendedMapPanel))
        {
            return;
        }

        // Get texture data
        Texture2D const* textureAtlas{dtb::getTextureAtlas()};
        Vector2 texturePosition{dtb::getTexturePosition(graphic.renderID())};
        Vector2 tileSize{Textures::TILE_DIMENSIONS};
        Vector2 tileCenter{Vector2Scale(tileSize, 0.5F)};

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
        // Draw texture (using 0.5F pixel offset to get rid of texture bleeding)
        DrawTexturePro(
            *textureAtlas,
            Rectangle{
                texturePosition.x + 0.5F,
                texturePosition.y + 0.5F,
                Textures::TEXTURE_WIDTH - (2 * 0.5F),
                Textures::TEXTURE_WIDTH - (2 * 0.5F)},
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
