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
            for (auto& steppedTiles : TileMapFilters::filterReachable(
                     world.currentMap(),
                     unit.movement.range,
                     unit.position.tilePosition()))
            {
                for (auto& steppedTile : steppedTiles)
                {
                    // Add reachable tile to overlay
                    world.mapOverlay().createOrUpdate(
                        steppedTile.tile->position.tilePosition(),
                        Tile(
                            steppedTile.tile->position.tilePosition(),
                            Graphic(
                                RenderID::REACHABLE,
                                LayerID::MAP_OVERLAY)));
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

        for (auto& steppedTile : path)
        {
            world.framedMapOverlay().createOrUpdate(
                steppedTile.tile->position.tilePosition(),
                Tile(
                    steppedTile.tile->position.tilePosition(),
                    Graphic(
                        RenderID::PATH,
                        LayerID::MAP_OVERLAY)));
        }

        return path;
    }

}