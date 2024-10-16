#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <unordered_set>

//* SoA class
//* Containers need to stay in sync, therefore they are private and direct getters are provided to prevent individual inserts/deletions
class Tiles
{
    snx::DenseMap<Vector2I, PositionComponent> positions_{};

    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, VisibilityID> visibilityIDs_{};
    std::unordered_set<Vector2I> isSolids_{};
    std::unordered_set<Vector2I> isOpaques_{};

    RectangleExI mapSize_{};

public:
    //* Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityID visibilityID = VisibilityID::invisible);

    snx::DenseMap<Vector2I, PositionComponent> const& getPositions() const;

    snx::DenseMap<Vector2I, RenderID> const& getRenderIDs() const;
    snx::DenseMap<Vector2I, RenderID>& getRenderIDs();

    snx::DenseMap<Vector2I, VisibilityID> const& getVisibilityIDs() const;
    snx::DenseMap<Vector2I, VisibilityID>& getVisibilityIDs();

    std::unordered_set<Vector2I> const& getIsSolids() const;
    std::unordered_set<Vector2I>& getIsSolids();

    std::unordered_set<Vector2I> const& getIsOpaques() const;
    std::unordered_set<Vector2I>& getIsOpaques();

    RectangleExI mapSize() const;

private:
    void updateMapSize(Vector2I const& tilePosition);
};

#endif
