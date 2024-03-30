#include "MapOverlay.h"

#include "Graphic.h"
#include "LayerID.h"
#include "Pathfinder.h"
#include "RenderID.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileMapFilters.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "World.h"
#include "raylibEx.h"

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
                      LayerID::MAP_OVERLAY),
                    VisibilityID::VISIBLE));
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
                  LayerID::MAP_OVERLAY),
                VisibilityID::VISIBLE));
        }
    }

    Path& showPath(Vector2I unitPosition, Vector2I cursorPosition, int unitRange, World& world, bool& isPathShown)
    {
        static Vector2I origin{};
        static Vector2I target{};
        static Path path{};
        TileMap overlayTiles{world.mapOverlay()};

        // Check if path input changed
        if (!(
              origin == unitPosition
              && target == cursorPosition))
        {
            // Update input and path
            origin = unitPosition;
            target = cursorPosition;

            path = findPath(
              overlayTiles,
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
                  LayerID::MAP_OVERLAY),
                VisibilityID::VISIBLE));
        }

        return path;
    }
}