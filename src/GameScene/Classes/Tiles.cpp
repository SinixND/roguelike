#include "Tiles.h"

#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <string>

void Tiles::insert(
    Vector2I const& tilePosition,
    RenderID renderID,
    std::string const& tag,
    bool isSolid,
    bool blocksVision)
{
    position_.insert(tilePosition, Position{tilePosition});
    renderID_.insert(tilePosition, renderID);
    tag_.insert(tilePosition, tag);
    visibilityID_.insert(tilePosition, VisibilityID::unseen);
    isSolid_.insert(tilePosition, isSolid);
    blocksVision_.insert(tilePosition, blocksVision);
};

void Tiles::erase(Vector2I const& tilePosition)
{
    position_.erase(tilePosition);
    renderID_.erase(tilePosition);
    tag_.erase(tilePosition);
    visibilityID_.erase(tilePosition);
    isSolid_.erase(tilePosition);
    blocksVision_.erase(tilePosition);
};

Position& Tiles::position(Vector2I const& tilePosition)
{
    return *position_.at(tilePosition);
};

std::string const& Tiles::tag(Vector2I const& tilePosition)
{
    return *tag_.at(tilePosition);
};

RenderID Tiles::renderID(Vector2I const& tilePosition)
{
    return *renderID_.at(tilePosition);
};

VisibilityID Tiles::visibilityID(Vector2I const& tilePosition)
{
    return *visibilityID_.at(tilePosition);
};

bool Tiles::isSolid(Vector2I const& tilePosition)
{
    return *isSolid_.at(tilePosition);
};

bool Tiles::blocksVision(Vector2I const& tilePosition)
{
    return *blocksVision_.at(tilePosition);
};