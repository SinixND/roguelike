#include "ShowPath.h"

#include "Pathfinder.h"
#include "RuntimeDatabase.h"
#include "World.h"
#include <cstddef>

void showPath(bool& moveRangeShown, const Vector2Int& unitPosition, const Vector2Int& cursorPosition, size_t unitRange, World& world)
{
    // Show path
    if (moveRangeShown)
    {
        static Vector2Int origin{};
        static Vector2Int target{};
        static size_t range{};
        static std::vector<SteppedPosition> path{};

        // Check if path input changed
        if (!(
                origin == unitPosition &&
                target == cursorPosition &&
                range == unitRange))
        {
            // Update input and path
            origin = unitPosition;
            target = cursorPosition;
            range = unitRange;

            path = findPath(
                world.mapOverlay(),
                unitPosition,
                cursorPosition,
                unitRange);

            if (path.size()) dtb::Globals::setPathShown(true);
        }

        for (auto& steppedPosition : path)
        {
            Tile pathTile{};
            pathTile.setPosition(steppedPosition.position);
            pathTile.graphic().setRenderID(RenderID::path);
            pathTile.setIsSolid(false);

            world.framedMapOverlay().createOrUpdate(
                steppedPosition.position,
                pathTile);
        }
    }
};