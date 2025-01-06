#include "Tiles.h"

#include "Convert.h"
#include "RenderId.h"
#include "SparseSet.h"
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
        size_t tileId{0};

        if (!tiles.ids.contains(tilePosition))
        {
            tileId = tiles.idManager.requestId();
            tiles.ids.insert(tilePosition, tileId);
        }
        else
        {
            tileId = tiles.ids.at(tilePosition);
        }

        tiles.positions[tileId] = Convert::tileToWorld(tilePosition);

        tiles.renderIds[tileId] = renderId;

        tiles.visibilityIds[tileId] = visibilityId;

        if (isSolid)
        {
            tiles.isSolids.insert(tileId);
        }

        else
        {
            tiles.isSolids.erase(tileId);
        }

        if (isOpaque)
        {
            tiles.isOpaques.insert(tileId);
        }

        else
        {
            tiles.isOpaques.erase(tileId);
        }

        updateMapSize(
            tiles,
            tilePosition);
    }
}
