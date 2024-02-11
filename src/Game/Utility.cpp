#include "Utility.h"

#include "RuntimeDatabase.h"
#include "raylib.h"
#include <raylib.h>

Vector2Int getScreenToTile(const Vector2& pixel)
{
    Vector2 worldPixel{GetScreenToWorld2D(pixel, dtb::Globals::getCamera())};

    return Vector2Int{
        static_cast<int>(std::floor((worldPixel.x + (dtb::Constants::getTileSize() / 2)) / dtb::Constants::getTileSize())),
        static_cast<int>(std::floor((worldPixel.y + (dtb::Constants::getTileSize() / 2)) / dtb::Constants::getTileSize()))};
}

Vector2 getTileToWorld(const Vector2Int& position)
{
    return Vector2{
        (position.x * dtb::Constants::getTileSize()),
        (position.y * dtb::Constants::getTileSize())};
}

void setMouseTile(Vector2Int& position)
{
    Vector2Int newPosition{
        getScreenToTile(GetMousePosition())};

    position.x = newPosition.x;
    position.y = newPosition.y;
}