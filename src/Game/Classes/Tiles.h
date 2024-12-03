#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <unordered_set>

class Tiles
{
    RectangleExI mapSize_{};

public:
    snx::DenseMap<Vector2I, PositionComponent> positions{};
    snx::DenseMap<Vector2I, RenderID> renderIDs{};
    snx::DenseMap<Vector2I, VisibilityID> visibilityIDs{};
    std::unordered_set<Vector2I> isSolids{};
    std::unordered_set<Vector2I> isOpaques{};

public:
    //* Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityID visibilityID = VisibilityID::INVISIBLE);

    bool isSolid(Vector2I const& tilePosition) const;
    bool isOpaque(Vector2I const& tilePosition) const;

public:
    RectangleExI mapSize() const;

private:
    void updateMapSize(Vector2I const& tilePosition);
};

#endif
