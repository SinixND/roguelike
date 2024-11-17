#include "Tiles.h"
#include "Convert.h"
#include "DenseMap.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>
#include <unordered_set>

void updateMapSize(RectangleExI* mapSize, Vector2I const& tilePosition)
{
    *mapSize = RectangleExI{
        Vector2I{
            std::min(tilePosition.x, mapSize->left()),
            std::min(tilePosition.y, mapSize->top())},
        Vector2I{
            std::max(tilePosition.x, mapSize->right()),
            std::max(tilePosition.y, mapSize->bottom())}};
}

void createTile(
    Tiles* tiles,
    Vector2I const& tilePosition,
    RenderId renderId,
    bool isSolid,
    bool isOpaque,
    VisibilityId visibilityId)
{
    tiles->positions[tilePosition] = Convert::tileToWorld(tilePosition);

    tiles->renderIds[tilePosition] = renderId;

    tiles->visibilityIds[tilePosition] = visibilityId;

    if (isSolid)
    {
        tiles->solids.insert(tilePosition);
    }
    else
    {
        tiles->solids.erase(tilePosition);
    }

    if (isOpaque)
    {
        tiles->opaques.insert(tilePosition);
    }
    else
    {
        tiles->opaques.erase(tilePosition);
    }

    updateMapSize(&(tiles->mapSize), tilePosition);
}
