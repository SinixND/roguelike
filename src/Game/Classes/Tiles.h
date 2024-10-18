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
//* Its the clients responsibility to avoid desync caused by individual size modifications of DenseMaps (eg. insert, erase)
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
        VisibilityID visibilityID = VisibilityID::invisible);

    bool isSolid(Vector2I const& tilePosition) const;
    bool isOpaque(Vector2I const& tilePosition) const;

    RectangleExI mapSize() const;

private:
    void updateMapSize(Vector2I const& tilePosition);
};

#endif
