#include "Tiles.h"
#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>
#include <unordered_set>
#include <utility>

void Tiles::updateMapSize(Vector2I const& tilePosition)
{
    mapSize_ = RectangleExI{
        Vector2I{
            std::min(tilePosition.x, mapSize_.left()),
            std::min(tilePosition.y, mapSize_.top())},
        Vector2I{
            std::max(tilePosition.x, mapSize_.right()),
            std::max(tilePosition.y, mapSize_.bottom())}};
}

void Tiles::create(
    Vector2I const& tilePosition,
    RenderId renderId,
    bool isSolid,
    bool isOpaque,
    VisibilityId visibilityId)
{
    positions[tilePosition].changeTo(tilePosition);

    renderIds[tilePosition] = renderId;

    visibilityIds[tilePosition] = visibilityId;

    if (isSolid)
    {
        isSolids.insert(tilePosition);
    }

    else
    {
        isSolids.erase(tilePosition);
    }

    if (isOpaque)
    {
        isOpaques.insert(tilePosition);
    }

    else
    {
        isOpaques.erase(tilePosition);
    }

    updateMapSize(tilePosition);
}

RectangleExI Tiles::mapSize() const
{
    return mapSize_;
}
