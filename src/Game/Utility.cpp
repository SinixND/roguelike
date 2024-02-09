#include "Utility.h"

#include "RuntimeDatabase.h"
#include "raylibEx.h"
#include <raylib.h>

Vector2Int getScreenToTile(const Vector2& pixel)
{
    Vector2 worldPixel{GetScreenToWorld2D(pixel, dtb::Globals::getCamera())};

    return Vector2Int{
        static_cast<int>(std::floor((worldPixel.x + (dtb::Constants::getTileSize() / 2)) / dtb::Constants::getTileSize())),
        static_cast<int>(std::floor((worldPixel.y + (dtb::Constants::getTileSize() / 2)) / dtb::Constants::getTileSize()))};
};

Vector2 getTileToScreen(const Vector2Int& position)
{
    Vector2 worldPixel{
        (position.x * dtb::Constants::getTileSize()) - (dtb::Constants::getTileSize() / 2),
        (position.y * dtb::Constants::getTileSize()) - (dtb::Constants::getTileSize() / 2)};

    return GetWorldToScreen2D(worldPixel, dtb::Globals::getCamera());
};
