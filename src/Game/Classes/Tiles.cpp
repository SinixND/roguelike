#include "Tiles.h"

#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <algorithm>
#include <unordered_set>
#include <utility>

void Tiles::set(
    Vector2I const& tilePosition,
    RenderID renderID,
    bool isSolid,
    bool isOpaque,
    VisibilityID visibilityID)
{
    positions[tilePosition].changeTo(tilePosition);

    renderIDs[tilePosition] = renderID;

    visibilityIDs[tilePosition] = visibilityID;

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

bool Tiles::isSolid(Vector2I const& tilePosition) const
{
    return isSolids.contains(tilePosition);
}

bool Tiles::isOpaque(Vector2I const& tilePosition) const
{
    return isOpaques.contains(tilePosition);
}

RectangleExI Tiles::mapSize() const
{
    return mapSize_;
}
