#include "Render.h"

#include "Constants.h"
#include "Graphic.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

Rectangle getRenderArea()
{
    return Rectangle{
        -MAP_RENDER_AREA_MARGIN + LEFT_MAP_RENDER_OFFSET,
        -MAP_RENDER_AREA_MARGIN + TOP_MAP_RENDER_OFFSET,
        GetRenderWidth() - LEFT_MAP_RENDER_OFFSET - RIGHT_MAP_RENDER_OFFSET + 2 * MAP_RENDER_AREA_MARGIN,
        GetRenderHeight() - TOP_MAP_RENDER_OFFSET - BOTTOM_MAP_RENDER_OFFSET + 2 * MAP_RENDER_AREA_MARGIN};
}

void render(const Vector2& position, Graphic& graphic)
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
        WHITE);
}
