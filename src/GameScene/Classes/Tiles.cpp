#include "Tiles.h"

#include "DenseMap.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <algorithm>
#include <cstddef>
#include <string>
#include <unordered_set>

void Tiles::set(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& tag,
    VisibilityID visibilityID,
    bool isSolid,
    bool isOpaque)
{
    positions_[tilePosition] = Position{tilePosition};
    renderIDs_[tilePosition] = renderID;
    tags_[tilePosition] = tag;
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

// void Tiles::erase(Vector2I const& tilePosition)
// {
//     positions_.erase(tilePosition);
//     renderIDs_.erase(tilePosition);
//     tags_.erase(tilePosition);
//     visibilityIDs_.erase(tilePosition);
//     isSolids_.erase(tilePosition);
//     isOpaques_.erase(tilePosition);
// }

snx::DenseMap<Vector2I, Position>& Tiles::positions() { return positions_; };

Position const& Tiles::position(Vector2I const& tilePosition)
{
    return positions_[tilePosition];
}

std::string const& Tiles::tag(Vector2I const& tilePosition)
{
    return tags_[tilePosition];
}

snx::DenseMap<Vector2I, RenderID>& Tiles::renderIDs()
{
    return renderIDs_;
};

RenderID Tiles::renderID(Vector2I const& tilePosition)
{
    return renderIDs_[tilePosition];
}

snx::DenseMap<Vector2I, VisibilityID>& Tiles::visibilityIDs()
{
    return visibilityIDs_;
}

VisibilityID Tiles::visibilityID(Vector2I const& tilePosition)
{
    return visibilityIDs_[tilePosition];
}
void Tiles::setVisibilityID(
    Vector2I const& tilePosition,
    VisibilityID visibilityID)
{
    visibilityIDs_[tilePosition] = visibilityID;
}

bool Tiles::isSolid(Vector2I const& tilePosition)
{
    return isSolids_.contains(tilePosition);
}

std::unordered_set<Vector2I> const& Tiles::isOpaques()
{
    return isOpaques_;
}

bool Tiles::isOpaque(Vector2I const& tilePosition)
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
};

size_t Tiles::size()
{
    return positions_.size();
}

RectangleExI Tiles::mapSize()
{
    return mapSize_;
}
