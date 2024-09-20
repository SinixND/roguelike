#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <unordered_set>

class Tiles
{
    snx::DenseMap<Vector2I, Position> positions_{};
    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, VisibilityID> visibilityIDs_{};
    std::unordered_set<Vector2I> isSolids_{};
    std::unordered_set<Vector2I> isOpaques_{};

    RectangleExI mapSize_{};

public:
    // Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityID visibilityID = VisibilityID::invisible);

    // void erase(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, Position> const& positions() const;
    Position const& position(Vector2I const& tilePosition) const;

    snx::DenseMap<Vector2I, RenderID> const& renderIDs() const;
    RenderID renderID(Vector2I const& tilePosition) const;

    snx::DenseMap<Vector2I, VisibilityID> const& visibilityIDs() const;
    VisibilityID visibilityID(Vector2I const& tilePosition) const;
    void setVisibilityID(Vector2I const& tilePosition, VisibilityID visibilityID);

    bool isSolid(Vector2I const& tilePosition) const;

    std::unordered_set<Vector2I> const& isOpaques() const;
    bool isOpaque(Vector2I const& tilePosition) const;

    RectangleExI mapSize() const;

    size_t size() const;

private:
    void updateMapSize(Vector2I const& tilePosition);
};

#endif
