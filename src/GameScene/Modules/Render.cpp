#include "Render.h"

#include "Graphic.h"
#include "Panels.h"
#include "TextureData.h"
#include "Textures.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace Render
{
    void adjustTint(
        Color& tint,
        VisibilityID visibilityID,
        bool cheatMode)
    {
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

            if (cheatMode)
            {
                tint = RED;
                tint = ColorBrightness(tint, 0.67);
            }
            break;
        }
    }

    void render(
        RectangleEx const& mapPanel,
        Textures& gameTextures,
        Vector2 texturePosition,
        Vector2 position,
        Vector2 tileSize,
        Vector2 tileCenter,
        Color const& tint)
    {
        BeginScissorMode(
            mapPanel.left(),
            mapPanel.top(),
            mapPanel.width(),
            mapPanel.height());

        // Draw texture (using 0.5F pixel offset to get rid of texture bleeding)
        DrawTexturePro(
            *gameTextures.getTextureAtlas(),
            Rectangle{
                texturePosition.x + 0.5F,
                texturePosition.y + 0.5F,
                TextureData::TEXTURE_WIDTH - (2 * 0.5F),
                TextureData::TEXTURE_WIDTH - (2 * 0.5F)},
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

    void update(
        Vector2 position,
        GraphicComponent graphic,
        Camera2D const& camera,
        Textures& gameTextures,
        bool cheatMode,
        VisibilityID visibilityID)
    {
        static RectangleEx mapPanel{PanelMap::setup()};
        static RectangleEx extendedMapPanel{mapPanel};
        extendedMapPanel
            .offsetLeft(-TextureData::TILE_SIZE)
            .offsetTop(-TextureData::TILE_SIZE)
            .offsetRight(TextureData::TILE_SIZE)
            .offsetBottom(TextureData::TILE_SIZE);

        if (IsWindowResized())
        {
            mapPanel = PanelMap::setup();
            extendedMapPanel = mapPanel;
            extendedMapPanel
                .offsetLeft(-TextureData::TILE_SIZE)
                .offsetTop(-TextureData::TILE_SIZE)
                .offsetRight(TextureData::TILE_SIZE)
                .offsetBottom(TextureData::TILE_SIZE);
        }

        // Skip if pixel is out of render area
        if (!CheckCollisionPointRec(GetWorldToScreen2D(position, camera), extendedMapPanel))
        {
            return;
        }

        // Get texture data
        Vector2 texturePosition{gameTextures.getTexturePosition(graphic.renderID())};
        Vector2 tileSize{TextureData::TILE_DIMENSIONS};
        Vector2 tileCenter{Vector2Scale(tileSize, 0.5F)};

        // Consider visibility
        Color tint{WHITE};

        adjustTint(
            tint,
            visibilityID,
            cheatMode);

        render(
            mapPanel,
            gameTextures,
            texturePosition,
            position,
            tileSize,
            tileCenter,
            tint);
    }
}
