#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "IdManager.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <unordered_set>

struct Tiles
{
    snx::IdManager idManager{};
    snx::DenseMap<Vector2I, size_t> ids{};

    snx::SparseSet<Vector2> positions{};
    snx::SparseSet<RenderId> renderIds{};
    snx::SparseSet<VisibilityId> visibilityIds{};

    std::unordered_set<size_t> isSolids{};
    std::unordered_set<size_t> isOpaques{};

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
