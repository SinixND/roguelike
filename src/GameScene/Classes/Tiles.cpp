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

void Tiles::insert(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& tag,
    VisibilityID visibilityID,
    bool isSolid,
    bool blocksVision)
{
    positions_.insert(tilePosition, Position{tilePosition});
    renderIDs_.insert(tilePosition, renderID);
    tags_.insert(tilePosition, tag);
    visibilityIDs_.insert(tilePosition, visibilityID);

    if (isSolid)
    {
        isSolids_.insert(tilePosition);
    }

    if (blocksVision)
    {
        blocksVisions_.insert(tilePosition);
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
//     blocksVisions_.erase(tilePosition);
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

std::unordered_set<Vector2I> const& Tiles::blocksVisions()
{
    return blocksVisions_;
}

bool Tiles::blocksVision(Vector2I const& tilePosition)
{
    return blocksVisions_.contains(tilePosition);
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