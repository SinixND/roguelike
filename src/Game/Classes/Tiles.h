#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <unordered_set>

struct Tiles
{
    snx::DenseMap<Vector2I, PositionComponent> positions{};
    snx::DenseMap<Vector2I, RenderId> renderIds{};
    snx::DenseMap<Vector2I, VisibilityId> visibilityIds{};
    std::unordered_set<Vector2I> isSolids{};
    std::unordered_set<Vector2I> isOpaques{};

    RectangleExI mapSize{};
};

namespace TilesModule
{
    void createSingle(
        Tiles& tiles,
        Vector2I const& tilePosition,
        RenderId renderId,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityId visibilityId = VisibilityId::INVISIBLE);
}

#endif
