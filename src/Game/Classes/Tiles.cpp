#include "Tiles.h"

#include "DenseMap.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <algorithm>
#include <cstddef>
#include <unordered_set>

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

snx::DenseMap<Vector2I, Position> const& Tiles::positions() const
{
    return positions_;
}

Position const& Tiles::position(Vector2I const& tilePosition) const
{
    return positions_.at(tilePosition);
}

snx::DenseMap<Vector2I, RenderID> const& Tiles::renderIDs() const
{
    return renderIDs_;
}

RenderID Tiles::renderID(Vector2I const& tilePosition) const
{
    return renderIDs_.at(tilePosition);
}

snx::DenseMap<Vector2I, VisibilityID> const& Tiles::visibilityIDs() const
{
    return visibilityIDs_;
}

VisibilityID Tiles::visibilityID(Vector2I const& tilePosition) const
{
    return visibilityIDs_.at(tilePosition);
}

void Tiles::setVisibilityID(
    Vector2I const& tilePosition,
    VisibilityID visibilityID)
{
    visibilityIDs_.at(tilePosition) = visibilityID;
}

bool Tiles::isSolid(Vector2I const& tilePosition) const
{
    return isSolids_.contains(tilePosition);
}

std::unordered_set<Vector2I> const& Tiles::isOpaques() const
{
    return isOpaques_;
}

bool Tiles::isOpaque(Vector2I const& tilePosition) const
{
    return isOpaques_.contains(tilePosition);
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

size_t Tiles::size() const
{
    return positions_.size();
}

RectangleExI Tiles::mapSize() const
{
    return mapSize_;
}
