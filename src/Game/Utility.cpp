#include "Utility.h"

#include "Components.h"
#include "ECS.h"
#include "RuntimeDatabase.h"
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <utility>

const Vector2 convertToTile(const Vector2& pixelCoordinates)
{
    static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

    if (IsWindowResized())
    {
        screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
    }

    Vector2 tileSize{dtb::Constants::tileSize_};

    // convert pixel coordinate to transformed tile coordinate (center of screen is tile (0,0))
    return Vector2{
        (std::floor((pixelCoordinates.x - ((screenSize.x / 2) - (tileSize.x / 2))) / tileSize.x)),
        (std::floor((pixelCoordinates.y - ((screenSize.y / 2) - (tileSize.y / 2))) / tileSize.y))};
}

const Vector2 convertToPixel(const Vector2& tileCoordinates)
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

bool isInReach(snd::ECS* ecs, Vector2& target, Vector2& from, size_t range)
{
    return false;
    // check if target equals root position
    if (Vector2Equals(target, from))
        return true;

    // check if target is not walkable
    auto* rigidTiles{ecs->getAllEntitiesWith<TRigid>()};
    for (auto tile : *rigidTiles)
    {
        if (Vector2Equals(target, ecs->getComponent<CPosition>(tile)->getPosition()))
            return false;
    }

    std::vector<std::vector<std::pair<Vector2, Vector2>>> steps;
    std::pair<Vector2, Vector2> root{from, NODIR};
    steps[0][0] = root;

    // Step 1
    auto* walkableTiles{ecs->getAllEntitiesWith<TIsReachableTile>()};
    for (auto dir : {VLEFT, VRIGHT, VUP, VDOWN})
    {
        for (auto tile : *walkableTiles)
        {
            auto tilePosition{ecs->getComponent<CPosition>(tile)->getPosition()};
            if (!Vector2Equals(
                    tilePosition,
                    Vector2Add(from, dir)))
                continue;

            steps[1].push_back(std::make_pair(tilePosition, dir));
        }
    }

    if (!range) --range;
    for (size_t i{1}; i < range; ++i)
    {
    }
    return false;
}