#include "Renderer.h"

#include "Globals/Constants.h"
#include "RenderId.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include <raylib.h>
#include <raymath.h>

void Renderer::render(RenderId renderId, int x, int y)
{
    Texture2D* texture{dtb::Textures::get(renderId)};

    Vector2 tileSize{TILE_DIMENSIONS};
    Vector2 tileCenter{Vector2Scale(tileSize, 0.5f)};

    Vector2 pixelCoordinates{
        positionToWorld(
            {x, y})};

    DrawTexturePro(
        *texture,
        Rectangle{
            0,
            0,
            float(texture->width),
            float(texture->height)},
        Rectangle{
            pixelCoordinates.x,
            pixelCoordinates.y,
            tileSize.x,
            tileSize.y},
        tileCenter,
        0,
        WHITE);
}
