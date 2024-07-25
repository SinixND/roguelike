#include "Tiles.h"

#include "GameCamera.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <algorithm>
#include <string>
#include <vector>

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

void Tiles::erase(Vector2I const& tilePosition)
{
    positions_.erase(tilePosition);
    renderIDs_.erase(tilePosition);
    tags_.erase(tilePosition);
    visibilityIDs_.erase(tilePosition);
    isSolids_.erase(tilePosition);
    blocksVisions_.erase(tilePosition);
}

std::vector<Position> const& Tiles::positions()
{
    return positions_.values();
}

Position const& Tiles::position(Vector2I const& tilePosition)
{
    return positions_[tilePosition];
}

std::string const& Tiles::tag(Vector2I const& tilePosition)
{
    return tags_[tilePosition];
}

std::vector<RenderID> const& Tiles::renderIDs()
{
    return renderIDs_.values();
}

RenderID Tiles::renderID(Vector2I const& tilePosition)
{
    return renderIDs_[tilePosition];
}

VisibilityID Tiles::visibilityID(Vector2I const& tilePosition)
{
    return visibilityIDs_[tilePosition];
}

bool Tiles::isSolid(Vector2I const& tilePosition)
{
    return isSolids_.contains(tilePosition);
}

bool Tiles::blocksVision(Vector2I const& tilePosition)
{
    return blocksVisions_.contains(tilePosition);
}

void Tiles::updateMapSize(Vector2I const& tilePosition)
{
    mapSize_ = RectangleExI{
        std::min(tilePosition.x, mapSize_.left()),
        std::max(tilePosition.x, mapSize_.right()),
        std::min(tilePosition.y, mapSize_.top()),
        std::max(tilePosition.y, mapSize_.bottom())};
}

