#include "ShowUnitRange.h"

#include "Pathfinder.h"
#include "Tile.h"
#include "Unit.h"
#include "World.h"

void showUnitRange(bool& moveRangeShown, Unit& unit, World& world)
{
    // Show range of selected unit
    if (unit.selected() && !moveRangeShown)
    {
        moveRangeShown = true;

        // Filter relevant tiles
        for (auto& steppedPositions : filterReachable(
                 world.currentMap(),
                 unit.move().range(),
                 unit.position()))
        {
            for (auto& steppedPosition : steppedPositions)
            {
                // Create reachable tile
                Tile reachableTile{};
                reachableTile.setPosition(steppedPosition.position);
                reachableTile.graphic().setRenderID(RenderID::reachable);
                reachableTile.setIsSolid(false);

                // Add reachable tile to overlay
                world.mapOverlay().createOrUpdate(
                    steppedPosition.position,
                    reachableTile);
            }
        }
    }
};