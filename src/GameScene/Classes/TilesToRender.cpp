#include "TilesToRender.h"

#include "Tiles.h"
#include "raylibEx.h"

void TilesToRender::insert(Tiles& currentMap, Vector2I const& tilePosition)
{
    renderPositions_.push_back(
        currentMap.positions()[tilePosition].renderPosition());

    renderIDs_.push_back(
        currentMap.renderIDs()[tilePosition]);

    visibilityIDs_.push_back(
        currentMap.visibilityIDs()[tilePosition]);
}

void TilesToRender::clear()
{
    renderPositions_.clear();
    renderIDs_.clear();
    visibilityIDs_.clear();
}