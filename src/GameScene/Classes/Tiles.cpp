#include "Tiles.h"

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
    ++tileCount_;
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
    return positions_.at(tilePosition);
}

std::string const& Tiles::tag(Vector2I const& tilePosition)
{
    return tags_.at(tilePosition);
}

std::vector<RenderID> const& Tiles::renderIDs()
{
    return renderIDs_.values();
}

RenderID Tiles::renderID(Vector2I const& tilePosition)
{
    return renderIDs_.at(tilePosition);
}

VisibilityID Tiles::visibilityID(Vector2I const& tilePosition)
{
    return visibilityIDs_.at(tilePosition);
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
        std::min(tilePosition.x, mapSize_.left),
        std::max(tilePosition.x, mapSize_.right),
        std::min(tilePosition.y, mapSize_.top),
        std::max(tilePosition.y, mapSize_.bottom)};
}

// void Tiles::updateTilesToRender(
//     GameCamera const& gameCamera,
//     RectangleEx const& mapPanel)
// {
//     snx::BM::start("updateTilesToRender");
//     RectangleExI renderRectangle{
//         Vector2SubtractValue(
//             UnitConversion::screenToTilePosition(
//                 mapPanel.topLeft(),
//                 gameCamera.get()),
//             1),
//         Vector2AddValue(
//             UnitConversion::screenToTilePosition(
//                 mapPanel.bottomRight(),
//                 gameCamera.get()),
//             1)};

//     /*
//     // for (size_t i{0}; i < tileCount_; ++i)
//     for (auto tilePosition : positions_.keys())
//     {
//         bool conditionMet{CheckCollisionPointRec(tilePosition, renderRectangle)};
//         bool existsInList{tilesToRender_.positions_.keys().contains(tilePosition)};

//         // Skip if condition is met and is in list
//         if (conditionMet && existsInList)
//         {
//             continue;
//         }

//         // Add if does not exist yet
//         else if (conditionMet)
//         {
//             tilesToRender_.positions_.insert(
//                 tilePosition,
//                 *positions_.at(tilePosition));

//             tilesToRender_.renderIDs_.insert(
//                 tilePosition,
//                 *renderIDs_.at(tilePosition));

//             tilesToRender_.visibilityIDs_.insert(
//                 tilePosition,
//                 *visibilityIDs_.at(tilePosition));
//         }

//         // Delete if condiiton not met
//         else if (existsInList)
//         {
//             tilesToRender_.positions_.erase(
//                 tilePosition);

//             tilesToRender_.renderIDs_.erase(
//                 tilePosition);

//             tilesToRender_.visibilityIDs_.erase(
//                 tilePosition);
//         }

//         tilesToRender_.tileCount_ = tilesToRender_.positions_.keys().size();
//     }
//     */

//     // Reset
//     tilesToRender_ = TilesToRender();

//     for (auto x{renderRectangle.left}; x <= renderRectangle.right; ++x)
//     {
//         for (auto y{renderRectangle.top}; y <= renderRectangle.bottom; ++y)
//         {
//             Vector2I tilePosition{x, y};

//             // if (!positions_.contains(tilePosition))
//             // {
//             //     continue;
//             // }

//             tilesToRender_.renderPositions_.push_back(
//                 positions_.at(tilePosition)->renderPosition());

//             tilesToRender_.renderIDs_.push_back(
//                 *renderIDs_.at(tilePosition));

//             tilesToRender_.visibilityIDs_.push_back(
//                 *visibilityIDs_.at(tilePosition));
//         }
//     }

//     tilesToRender_.tileCount_ = tilesToRender_.visibilityIDs_.size();
//     std::cout << tilesToRender_.tileCount_ << "\n";

//     snx::BM::end("updateTilesToRender");
// }
