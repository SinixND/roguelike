#include "TilePositionConversion.h"

#include "RuntimeDatabase.h"
#include <raylib.h>

Vector2Int screenToPosition(const Vector2& pixel)
{
    Vector2 worldPixel{GetScreenToWorld2D(pixel, dtb::Globals::camera())};

    return Vector2Int{
        static_cast<int>(std::floor((worldPixel.x + (dtb::Constants::tileSize() / 2)) / dtb::Constants::tileSize())),
        static_cast<int>(std::floor((worldPixel.y + (dtb::Constants::tileSize() / 2)) / dtb::Constants::tileSize()))};
}

Vector2 positionToWorld(const Vector2Int& position)
{
    return Vector2{
        (position.x * dtb::Constants::tileSize()),
        (position.y * dtb::Constants::tileSize())};
}

void setMouseTile(Vector2Int& position)
{
    Vector2Int newPosition{
        screenToPosition(GetMousePosition())};

    position.x = newPosition.x;
    position.y = newPosition.y;
}