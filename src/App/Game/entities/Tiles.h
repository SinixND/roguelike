#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "IdManager.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <unordered_set>

struct Tiles
{
    static inline snx::IdManager idManager{};

    snx::DenseMap<Vector2I, size_t> ids{};

    snx::DenseMap<size_t, Vector2> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, VisibilityId> visibilityIds{};
    std::unordered_set<size_t> isSolids{};
    std::unordered_set<size_t> isOpaques{};

    RectangleExI mapSize{};
};

namespace TilesModule
{
    [[nodiscard]]
    Tiles const& insert(
        Tiles& tiles,
        Vector2I const& tilePosition,
        RenderId renderId,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityId visibilityId = VisibilityId::INVISIBLE
    );
}
#endif
