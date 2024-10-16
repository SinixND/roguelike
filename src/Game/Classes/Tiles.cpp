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
    positions_[tilePosition].changeTo(tilePosition);

    renderIDs_[tilePosition] = renderID;

    visibilityIDs_[tilePosition] = visibilityID;

    if (isSolid)
    {
        isSolids_.insert(tilePosition);
    }

    else
    {
        isSolids_.erase(tilePosition);
    }

    if (isOpaque)
    {
        isOpaques_.insert(tilePosition);
    }

    else
    {
        isOpaques_.erase(tilePosition);
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

snx::DenseMap<Vector2I, PositionComponent> const& Tiles::getPositions() const
{
    return positions_;
}

snx::DenseMap<Vector2I, RenderID> const& Tiles::getRenderIDs() const
{
    return renderIDs_;
}

snx::DenseMap<Vector2I, RenderID>& Tiles::getRenderIDs()
{
    return const_cast<snx::DenseMap<Vector2I, RenderID>&>(std::as_const(*this).getRenderIDs());
}

snx::DenseMap<Vector2I, VisibilityID> const& Tiles::getVisibilityIDs() const
{
    return visibilityIDs_;
}

snx::DenseMap<Vector2I, VisibilityID>& Tiles::getVisibilityIDs()
{
    return const_cast<snx::DenseMap<Vector2I, VisibilityID>&>(std::as_const(*this).getVisibilityIDs());
}

std::unordered_set<Vector2I> const& Tiles::getIsSolids() const
{
    return isSolids_;
}

std::unordered_set<Vector2I>& Tiles::getIsSolids()
{
    return const_cast<std::unordered_set<Vector2I>&>(std::as_const(*this).getIsSolids());
}

std::unordered_set<Vector2I> const& Tiles::getIsOpaques() const
{
    return isOpaques_;
}

std::unordered_set<Vector2I>& Tiles::getIsOpaques()
{
    return const_cast<std::unordered_set<Vector2I>&>(std::as_const(*this).getIsOpaques());
}

RectangleExI Tiles::mapSize() const
{
    return mapSize_;
}
