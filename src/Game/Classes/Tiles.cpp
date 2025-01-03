#include "Tiles.h"
#include "DenseMap.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>

void createTile(
    Tiles& tiles,
    Vector2I const& tilePosition,
    RenderId renderId,
    bool isSolid,
    bool isOpaque,
    VisibilityId visibilityId)
{
    size_t id{tiles.idManager.requestId()};

    tiles.positions.emplace(id, tilePosition);

    tiles.renderIds.insert(id, renderId);

    tiles.visibilityIds.insert(id, visibilityId);

    if (isSolid)
    {
        tiles.isSolids.insert(id);
    }

    else
    {
        tiles.isSolids.erase(id);
    }

    if (isOpaque)
    {
        tiles.isOpaques.insert(id);
    }

    else
    {
        tiles.isOpaques.erase(id);
    }

    tiles.mapSize = RectangleExI{
        Vector2I{
            std::min(tilePosition.x, tiles.mapSize.left()),
            std::min(tilePosition.y, tiles.mapSize.top())},
        Vector2I{
            std::max(tilePosition.x, tiles.mapSize.right()),
            std::max(tilePosition.y, tiles.mapSize.bottom())}};
}
