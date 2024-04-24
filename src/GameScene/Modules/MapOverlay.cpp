#include "MapOverlay.h"

#include "GameObject.h"
#include "Graphic.h"
#include "LayerID.h"
#include "Pathfinder.h"
#include "Position.h"
#include "RenderID.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileMapFilters.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"

namespace
{
    static Path path_{};

    void showUnitMoveRange(Unit& unit, World& world)
    {
        // Filter relevant tiles
        for (auto& steppedTiles : TileMapFilters::filterMovable(
                 world.currentMap(),
                 unit.movementComponent.range(),
                 unit.positionComponent.tilePosition()))
        {
            for (auto& steppedTile : steppedTiles)
            {
                // Add reachable tile to overlay
                world.mapOverlay().createOrUpdate(
                    steppedTile.tile->positionComponent.tilePosition(),
                    GameObject(
                        PositionComponent(steppedTile.tile->positionComponent.tilePosition()),
                        GraphicComponent(
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
            unit.attackComponent.range(),
            unit.movementComponent.range(),
            unit.positionComponent.tilePosition())};

        for (auto& tile : inActionRangeTiles)
        {
            // Add reachable tile to overlay
            world.mapOverlay().createOrUpdate(
                tile->positionComponent.tilePosition(),
                GameObject(
                    PositionComponent(tile->positionComponent.tilePosition()),
                    GraphicComponent(
                        RenderID::ATTACKABLE,
                        LayerID::MAP_OVERLAY)));
        }
    }

    auto showPath(Vector2I unitPosition, Vector2I cursorPosition, int unitRange, World& world) -> Path&
    {
        static Vector2I origin{};
        static Vector2I target{};
        static Path path{};

        // Check if path input changed
        if (!(
                origin == unitPosition
                && target == cursorPosition))
        {
            // Update input and path
            origin = unitPosition;
            target = cursorPosition;

            path = findPath(
                world.currentMap(),
                unitPosition,
                cursorPosition,
                unitRange);
        }

        for (auto& steppedTile : path)
        {
            world.framedMapOverlay().createOrUpdate(
                steppedTile.tile->positionComponent.tilePosition(),
                GameObject(
                    PositionComponent(steppedTile.tile->positionComponent.tilePosition()),
                    GraphicComponent(
                        RenderID::PATH,
                        LayerID::MAP_OVERLAY)));
        }

        return path;
    }
}

namespace MapOverlay
{
    void update(Unit& hero, World& gameWorld, GameObject& cursor)
    {
        static bool isRangeShown{false};

        if (hero.isSelected())
        {
            if (!isRangeShown)
            {
                showUnitMoveRange(
                    hero,
                    gameWorld);

                showUnitActionRange(
                    hero,
                    gameWorld);

                isRangeShown = true;
            }
            else // range is shown
            {
                path_ = showPath(
                    hero.positionComponent.tilePosition(),
                    cursor.positionComponent.tilePosition(),
                    hero.movementComponent.range(),
                    gameWorld);
            }
        }
        else // not selected
        {
            if (isRangeShown)
            {
                gameWorld.mapOverlay().clear();

                isRangeShown = false;
            }
        }
    }

    auto path() -> Path&
    {
        return path_;
    }
}