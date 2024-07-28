#ifndef IG20240602211712
#define IG20240602211712

#include "DenseMap.h"
#include "Position.h"
#include "RenderID.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <string>
#include <unordered_set>

class Tiles
{
    snx::DenseMap<Vector2I, Position> positions_{};
    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, std::string> tags_{};
    snx::DenseMap<Vector2I, VisibilityID> visibilityIDs_{};
    std::unordered_set<Vector2I> isSolids_{};
    std::unordered_set<Vector2I> blocksVisions_{};

    RectangleExI mapSize_{};

private:
    void updateMapSize(Vector2I const& tilePosition);

public:
    void insert(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        VisibilityID visibilityID = VisibilityID::invisible,
        bool isSolid = false,
        bool blocksVision = false);

    // void erase(Vector2I const& tilePosition);

    size_t size();

    snx::DenseMap<Vector2I, Position>& positions();
    Position const& position(Vector2I const& tilePosition);

    std::string const& tag(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, RenderID>& renderIDs();
    RenderID renderID(Vector2I const& tilePosition);

    snx::DenseMap<Vector2I, VisibilityID>& visibilityIDs();
    VisibilityID visibilityID(Vector2I const& tilePosition);

    bool isSolid(Vector2I const& tilePosition);

    bool blocksVision(Vector2I const& tilePosition);

    RectangleExI mapSize();
};

#endif
