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

private:
    void updateMapSize(Vector2I const& tilePosition);

public:
    // Access or create
    void set(
        Vector2I const& tilePosition,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false,
        VisibilityID visibilityID = VisibilityID::invisible);

    // void erase(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, Position>& positions();
    Position const& position(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, RenderID>& renderIDs();
    RenderID renderID(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, VisibilityID>& visibilityIDs();
    VisibilityID visibilityID(Vector2I const& tilePosition);
    void setVisibilityID(Vector2I const& tilePosition, VisibilityID visibilityID);

    bool isSolid(Vector2I const& tilePosition);

    std::unordered_set<Vector2I> const& isOpaques();
    bool isOpaque(Vector2I const& tilePosition);

    RectangleExI mapSize();

    size_t size();
};

#endif