#ifndef IG20240602211712
#define IG20240602211712

#include "IdManager.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "VisibilityId.h"
#include "raylibEx.h"

struct Tiles
{
    snx::SparseSet<PositionComponent> positions{};
    snx::SparseSet<RenderId> renderIds{};
    snx::SparseSet<VisibilityId> visibilityIds{};
    //* Dont use bools with (underlying) std::vector
    snx::SparseSet<int> isSolids{};
    snx::SparseSet<int> isOpaques{};

    snx::IdManager idManager{};

    RectangleExI mapSize{};
};

//* Access or create
void createTile(
    Tiles& tiles,
    Vector2I const& tilePosition,
    RenderId renderId,
    bool isSolid = false,
    bool isOpaque = false,
    VisibilityId visibilityId = VisibilityId::INVISIBLE);
#endif
