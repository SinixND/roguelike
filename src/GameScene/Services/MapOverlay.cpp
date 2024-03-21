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
    void showUnitMoveRange(Unit& unit, World& world)
    {
        // Filter relevant tiles
        for (auto& steppedTiles : TileMapFilters::filterMovable(
                 world.currentMap(),
                 unit.movement.range(),
                 unit.transform.tilePosition()))
        {
            for (auto& steppedTile : steppedTiles)
            {
                // Add reachable tile to overlay
                world.mapOverlay().createOrUpdate(
                    steppedTile.tile->transform.tilePosition(),
                    Tile(
                        steppedTile.tile->transform.tilePosition(),
                        Graphic(
                            RenderID::REACHABLE,
                            LayerID::MAP_OVERLAY)));
            }
        }
    }

    void showUnitActionRange(Unit& unit, World& world)
    {
        // Filter relevant tiles
        auto inActionRangeTiles{TileMapFilters::filterInActionRange(
            world.currentMap(),
            unit.attack.range(),
            unit.movement.range(),
            unit.transform.tilePosition())};

        for (auto& tile : inActionRangeTiles)
        {
            // Add reachable tile to overlay
            world.mapOverlay().createOrUpdate(
                tile->transform.tilePosition(),
                Tile(
                    tile->transform.tilePosition(),
                    Graphic(
                        RenderID::ATTACKABLE,
                        LayerID::MAP_OVERLAY)));
        }
    }

    Path& showPath(Vector2i unitPosition, Vector2i cursorPosition, size_t unitRange, World& world, bool& isPathShown)
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

            if (!path.empty()) isPathShown = true;
        }

        for (auto& steppedTile : path)
        {
            world.framedMapOverlay().createOrUpdate(
                steppedTile.tile->transform.tilePosition(),
                Tile(
                    steppedTile.tile->transform.tilePosition(),
                    Graphic(
                        RenderID::PATH,
                        LayerID::MAP_OVERLAY)));
        }

        return path;
    }
}