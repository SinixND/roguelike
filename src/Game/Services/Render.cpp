#include "Render.h"

#include "Constants.h"
#include "RenderId.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void render(RenderId renderId, int x, int y)
{
    Vector2 positionPixel{
        positionToWorld(
            {x, y})};

    if (!IsPixelOnScreenRender(
            {GetWorldToScreen2D(
                positionPixel,
                dtb::Globals::camera())},
            3 * TILE_SIZE))
        return;

    Texture2D* texture{dtb::Textures::get(renderId)};

    Vector2 tileSize{TILE_DIMENSIONS};
    Vector2 tileCenter{Vector2Scale(tileSize, 0.5f)};

    DrawTexturePro(
        *texture,
        Rectangle{
            0,
            0,
            float(texture->width),
            float(texture->height)},
        Rectangle{
            positionPixel.x,
            positionPixel.y,
            tileSize.x,
            tileSize.y},
        tileCenter,
        0,
        WHITE);
}
