#include "Tiles.h"
#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>
#include <unordered_set>

void updateMapSize(
    Tiles& tiles,
    Vector2I const& tilePosition)
{
    tiles.mapSize = RectangleExI{
        Vector2I{
            std::min(tilePosition.x, tiles.mapSize.left()),
            std::min(tilePosition.y, tiles.mapSize.top())},
        Vector2I{
            std::max(tilePosition.x, tiles.mapSize.right()),
            std::max(tilePosition.y, tiles.mapSize.bottom())}};
}

namespace TilesModule
{
    void createSingle(
        Tiles& tiles,
        Vector2I const& tilePosition,
        RenderId renderId,
        bool isSolid,
        bool isOpaque,
        VisibilityId visibilityId)
    {
        PositionModule::changeTo(
            tiles.positions[tilePosition],
            tilePosition);

        tiles.renderIds[tilePosition] = renderId;

        tiles.visibilityIds[tilePosition] = visibilityId;

        if (isSolid)
        {
            tiles.isSolids.insert(tilePosition);
        }

        else
        {
            tiles.isSolids.erase(tilePosition);
        }

        if (isOpaque)
        {
            tiles.isOpaques.insert(tilePosition);
        }

        else
        {
            tiles.isOpaques.erase(tilePosition);
        }

        updateMapSize(
            tiles,
            tilePosition);
    }
}
