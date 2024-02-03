#ifndef ACTIONRENDER_H_20240128161353
#define ACTIONRENDER_H_20240128161353

#include "RuntimeDatabase.h"
#include "Utility.h"
#include <raylibEx.h>
#include <raymath.h>

inline void renderAction(const Texture2D* texture, const Vector2Int& position, const Vector2Int& transform)
{
    // Action
    Vector2 tileSize{dtb::Constants::tileSize_};
    Vector2 tileCenter{Vector2Scale(tileSize, 0.5)};

    Vector2 pixelCoordinates{
        Vector2Subtract(
            convertToPixel(
                Vector2Subtract(
                    position,
                    transform)),
            tileSize)};

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

#endif