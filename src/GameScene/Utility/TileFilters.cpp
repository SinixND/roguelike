#include "TileFilters.h"
#include "DenseMap.h"
#include "raylibEx.h"
#include "Tiles.h"
#include "TilesToRender.h"
#include "VisibilityID.h"
#include <vector>

std::vector<Vector2I> tilePositionsToRender(
    // World const& world,
    snx::DenseMap<Vector2I, VisibilityID>& visibilityIDs,
    RectangleExI const& renderRectangle)
{
    // Tiles& currentMap{world.currentMap()};
    // RectangleExI renderRect{renderRectangleExI()};
    Vector2I initialTilePositionToCheck{renderRectangle.topLeft()};
    std::vector<Vector2I> tilePositionsToReturn{};

    for (int x{0}; x < renderRectangle.width(); ++x)
    {
        for (int y{0}; y < renderRectangle.height(); ++y)
        {
            Vector2I tilePositionToCheck{
                initialTilePositionToCheck.x + x,
                initialTilePositionToCheck.y + y};

            if (
                !visibilityIDs.contains(tilePositionToCheck)
                // || (visibilityIDs[tilePositionToCheck] == VisibilityID::invisible)
            )
            {
                continue;
            }

            tilePositionsToReturn.push_back(tilePositionToCheck);
        }
    }

    return tilePositionsToReturn;
};

void TileFilters::initTilesToRender(
    TilesToRender& tilesToRender,
    Tiles& currentMap,
    RectangleExI const& renderRectangle)
{
    tilesToRender.clear();
    // Tiles& currentMap{world_.currentMap()};
    std::vector<Vector2I> tilePositions{
        tilePositionsToRender(
            currentMap.visibilityIDs(),
            renderRectangle)};

    for (auto& tilePosition : tilePositions)
    {
        tilesToRender.insert(currentMap, tilePosition);
    }
};

// void TileFilters::updateTilesToRender(std::vector<Vector2I>  const& tilePositions)
// {
//
// }
