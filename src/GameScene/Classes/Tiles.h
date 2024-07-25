#ifndef IG20240602211712
#define IG20240602211712

#include "GameCamera.h"
#include "Position.h"
#include "RenderID.h"
#include "DenseMap.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <string>
#include <unordered_set>
#include <vector>

class Tiles
{
    RectangleExI mapSize_{};

    snx::DenseMap<Vector2I, Position> positions_{};
    snx::DenseMap<Vector2I, RenderID> renderIDs_{};
    snx::DenseMap<Vector2I, std::string> tags_{};
    snx::DenseMap<Vector2I, VisibilityID> visibilityIDs_{};
    std::unordered_set<Vector2I> isSolids_{};
    std::unordered_set<Vector2I> blocksVisions_{};

public:
    void insert(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        VisibilityID visibilityID = VisibilityID::invisible,
        bool isSolid = false,
        bool blocksVision = false);

    void erase(Vector2I const& tilePosition);

    std::vector<Position> const& positions();
    Position const& position(Vector2I const& tilePosition);

    std::string const& tag(Vector2I const& tilePosition);

    std::vector<RenderID> const& renderIDs();
    RenderID renderID(Vector2I const& tilePosition);

    VisibilityID visibilityID(Vector2I const& tilePosition);

    bool isSolid(Vector2I const& tilePosition);

    bool blocksVision(Vector2I const& tilePosition);

    void updateMapSize(Vector2I const& tilePosition);
};

#endif
