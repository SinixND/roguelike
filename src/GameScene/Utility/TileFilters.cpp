#include "TileFilters.h"

namespace TileFilters
{
    std::vector<Vector2I> tilePositionsToRender();

    void initTilesToRender(TilesToRender& tilesToRender, Tiles const& currentMap, RectangleExI const& renderRectangle)
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
    
    std::vector<Vector2I> tilePositionsToRender(
        // World const& world, 
        snx::DenseMap<Vector2I, VisibilityID> const& visibilityIDs,
        RectangleExI const& renderRectangle);
    {
        // Tiles& currentMap{world.currentMap()};
        // RectangleExI renderRect{renderRectangleExI()};
        Vector2I initialTilePositionToCheck{renderRectangle.topLeft()};
        std::vector<Vector2I> tilePositionsToReturn{};

        for (int x{0}; x < renderRect.width(); ++x)
        {
            for (int y{0}; y < renderRect.height(); ++y)
            {
                Vector2I tilePositionToCheck{
                    initialTilePositionToCheck.x + x, 
                    initialTilePositionToCheck.y + y};

                if (
                    !visibilityIDs.contains(tilePositionToCheck)
                    || (visibilityIDs[tilePositionToCheck] == VisibilityID::invisible)
                )
                {
                    continue;
                }

                tilePositionsToReturn.push_back(tilePositionToCheck);
            }
        }

        return tilePositionsToReturn;
    };

    // void updateTilesToRender(std::vector<Vector2I>  const& tilePositions)
    // {
    //
    // }
}

#endif