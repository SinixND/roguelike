#include "MapOverlay.h"

#include "Graphic.h"
#include "LayerID.h"
#include "Pathfinder.h"
#include "RenderID.h"
#include "Tile.h"
#include "TileMapFilters.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <cstddef>

namespace MapOverlay
{
    void showUnitRange(bool& moveRangeShown, Unit& unit, World& world)
    {
        // Show range of selected unit
        if (unit.isSelected && !moveRangeShown)
        {
            moveRangeShown = true;

            // Filter relevant tiles
            for (auto& steppedPositions : TileMapFilters::filterReachable(
                     world.currentMap(),
                     unit.movement.range,
                     unit.position.tilePosition()))
            {
                for (auto& steppedPosition : steppedPositions)
                {
                    // Add reachable tile to overlay
                    world.mapOverlay().createOrUpdate(
                        steppedPosition.tilePosition,
                        Tile(
                            steppedPosition.tilePosition,
                            Graphic(
                                RenderID::reachable,
                                LayerID::mapOverlay)));
                }
            }
        }
    }

    Path& showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown)
    {
        static Vector2i origin{};
        static Vector2i target{};
        static size_t range{};
        static Path path{};

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

            if (!path.empty()) pathShown = true;
        }

        for (auto& steppedPosition : path)
        {
            world.framedMapOverlay().createOrUpdate(
                steppedPosition.tilePosition,
                Tile(
                    steppedPosition.tilePosition,
                    Graphic(
                        RenderID::path,
                        LayerID::mapOverlay)));
        }

        return path;
    }

}