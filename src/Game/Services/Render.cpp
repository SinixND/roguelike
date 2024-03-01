#include "Render.h"

#include "Constants.h"
#include "Graphic.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void render(const Vector2& positionTile, Graphic& graphic)
{
    // Return if pixel is out of screen
    if (!IsPixelOnScreenRender(
            {GetWorldToScreen2D(
                positionTile,
                dtb::camera())},
            3 * TILE_SIZE))
        return;

    // Get texture data
    Texture2D* texture{dtb::getTexture(graphic.renderID)};
    Vector2 tileSize{TILE_DIMENSIONS};
    Vector2 tileCenter{Vector2Scale(tileSize, 0.5f)};

    // Draw texture
    DrawTexturePro(
        *texture,
        Rectangle{
            0,
            0,
            float(texture->width),
            float(texture->height)},
        Rectangle{
            positionTile.x,
            positionTile.y,
            tileSize.x,
            tileSize.y},
        tileCenter,
        0,
        WHITE);
}

void renderAtTile(const Vector2i& tilePosition, Graphic& graphic)
{
    render(tilePositionToWorld(tilePosition), graphic);
}
