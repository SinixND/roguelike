#include "TilesActive.h"

#include "DenseMap.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>

void TilesActive::set(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& tag,
    std::function<void()> event,
    VisibilityID visibilityID,
    bool isSolid,
    bool isOpaque)
{
    positions_[tilePosition] = Position{tilePosition};
    renderIDs_[tilePosition] = renderID;
    tags_[tilePosition] = tag;
    events_[tilePosition] = event;
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

// void TilesActive::erase(Vector2I const& tilePosition)
// {
//     positions_.erase(tilePosition);
//     renderIDs_.erase(tilePosition);
//     tags_.erase(tilePosition);
//     visibilityIDs_.erase(tilePosition);
//     isSolids_.erase(tilePosition);
//     isOpaques_.erase(tilePosition);
// }

snx::DenseMap<Vector2I, Position>& TilesActive::positions() { return positions_; };

Position const& TilesActive::position(Vector2I const& tilePosition)
{
    return positions_[tilePosition];
}

std::string const& TilesActive::tag(Vector2I const& tilePosition)
{
    return tags_[tilePosition];
}

snx::DenseMap<Vector2I, RenderID>& TilesActive::renderIDs()
{
    return renderIDs_;
};

RenderID TilesActive::renderID(Vector2I const& tilePosition)
{
    return renderIDs_[tilePosition];
}

snx::DenseMap<Vector2I, VisibilityID>& TilesActive::visibilityIDs()
{
    return visibilityIDs_;
}

VisibilityID TilesActive::visibilityID(Vector2I const& tilePosition)
{
    return visibilityIDs_[tilePosition];
}
void TilesActive::setVisibilityID(
    Vector2I const& tilePosition,
    VisibilityID visibilityID)
{
    visibilityIDs_[tilePosition] = visibilityID;
}

bool TilesActive::isSolid(Vector2I const& tilePosition)
{
    return isSolids_.contains(tilePosition);
}

std::unordered_set<Vector2I> const& TilesActive::isOpaques()
{
    return isOpaques_;
}

bool TilesActive::isOpaque(Vector2I const& tilePosition)
{
    return isOpaques_.contains(tilePosition);
}

void TilesActive::updateMapSize(Vector2I const& tilePosition)
{
    mapSize_ = RectangleExI{
        Vector2I{
            std::min(tilePosition.x, mapSize_.left()),
            std::min(tilePosition.y, mapSize_.top())},
        Vector2I{
            std::max(tilePosition.x, mapSize_.right()),
            std::max(tilePosition.y, mapSize_.bottom())}};
};

size_t TilesActive::size()
{
    return positions_.size();
}

RectangleExI TilesActive::mapSize()
{
    return mapSize_;
}
