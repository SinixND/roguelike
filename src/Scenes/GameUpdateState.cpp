#include "Game.h"

#include "Pathfinder.h"
#include "RuntimeDatabase.h"

void showUnitRange(bool& moveRangeShown, Unit& unit, World& world);
void showPath(const Vector2Int& unitPosition, const Vector2Int& cursorPosition, size_t unitRange, World& world);

void GameScene::updateState()
{
    static bool moveRangeShown{false};

    showUnitRange(
        moveRangeShown,
        hero,
        world);

    // Clear overlay if no unit selected
    if (!hero.isSelected && moveRangeShown)
    {
        world.mapOverlay().clear();
        moveRangeShown = false;
        //* pathShown = false;
    }

    if (moveRangeShown)
    {
        showPath(
            hero.position,
            cursor.position,
            hero.movement.range,
            world);
    }
}

void showUnitRange(bool& moveRangeShown, Unit& unit, World& world)
{
    // Show range of selected unit
    if (unit.isSelected && !moveRangeShown)
    {
        moveRangeShown = true;

        // Filter relevant tiles
        for (auto& steppedPositions : filterReachable(
                 world.currentMap(),
                 unit.movement.range,
                 unit.position))
        {
            for (auto& steppedPosition : steppedPositions)
            {
                // Create reachable tile
                Tile reachableTile{};
                reachableTile.position = steppedPosition.position;
                reachableTile.graphic.renderID = RenderID::reachable;
                reachableTile.isSolid = false;

                // Add reachable tile to overlay
                world.mapOverlay().createOrUpdate(
                    steppedPosition.position,
                    reachableTile);
            }
        }
    }
}

void showPath(const Vector2Int& unitPosition, const Vector2Int& cursorPosition, size_t unitRange, World& world)
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

        //* if (!path.empty()) pathShown = true;
    }

    for (auto& steppedPosition : path)
    {
        Tile pathTile{};
        pathTile.position = steppedPosition.position;
        pathTile.graphic.renderID = RenderID::path;
        pathTile.isSolid = false;

        world.framedMapOverlay().createOrUpdate(
            steppedPosition.position,
            pathTile);
    }
}