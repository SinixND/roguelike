#include "Utility.h"

#include "CPosition.h"
#include "ECS.h"
#include "EntityId.h"
#include "RuntimeDatabase.h"
#include "TIsSolid.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <raylibEx.h>
#include <raymath.h>
#include <unordered_set>
#include <utility>
#include <vector>

Vector2Int convertToTile(const Vector2& pixelCoordinates)
{
    static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

    if (IsWindowResized())
    {
        screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
    }

    Vector2 tileSize{dtb::Constants::tileSize_};

    // convert pixel coordinate to transformed tile coordinate (center of screen is tile (0,0))
    return Vector2Int{
        static_cast<int>(std::floor((pixelCoordinates.x - ((screenSize.x / 2) - (tileSize.x / 2.0f))) / tileSize.x)),
        static_cast<int>(std::floor((pixelCoordinates.y - ((screenSize.y / 2) - (tileSize.y / 2.0f))) / tileSize.y))};
}

Vector2 convertToPixel(const Vector2Int& tileCoordinates)
{
    static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

    if (IsWindowResized())
    {
        screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
    }

    Vector2 tileSize{dtb::Constants::tileSize_};

    // convert transformed tile coordinate to pixel coordinate [ tile (0,0) is center of screen]
    return Vector2{
        (tileCoordinates.x * tileSize.x) + (screenSize.x / 2) + (tileSize.x),
        (tileCoordinates.y * tileSize.y) + (screenSize.y / 2) + (tileSize.y)};
}
