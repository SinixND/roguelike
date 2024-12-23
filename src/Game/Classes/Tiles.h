#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <unordered_set>

class Tiles
{
    RectangleExI mapSize_{};

public:
    snx::DenseMap<Vector2I, PositionComponent> positions{};
    snx::DenseMap<Vector2I, RenderId> renderIds{};
    snx::DenseMap<Vector2I, VisibilityId> visibilityIds{};
    std::unordered_set<Vector2I> isSolids{};
    std::unordered_set<Vector2I> isOpaques{};

public:
    //* Access or create
    void create(
        Vector2I const& tilePosition,
        RenderId renderId,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityId visibilityId = VisibilityId::INVISIBLE);

    RectangleExI mapSize() const;

private:
    void updateMapSize(Vector2I const& tilePosition);
};

#endif
