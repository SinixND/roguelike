#ifndef IG20240602211712
#define IG20240602211712

#include "Position.h"
#include "RenderID.h"
#include "SparseSet.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <string>

class Tiles
{
    snx::SparseSet<Vector2I, Position> position_{};
    snx::SparseSet<Vector2I, RenderID> renderID_{};
    snx::SparseSet<Vector2I, std::string> tag_{};
    snx::SparseSet<Vector2I, VisibilityID> visibilityID_{};
    // Cannot store bool because vector of bool is not a vector
    snx::SparseSet<Vector2I, int> isSolid_{};
    // Cannot store bool because vector of bool is not a vector
    snx::SparseSet<Vector2I, int> blocksVision_{};

public:
    void insert(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        bool isSolid,
        bool blocksVision);

    void erase(Vector2I const& tilePosition);

    Position& position(Vector2I const& tilePosition);
    std::string const& tag(Vector2I const& tilePosition);
    RenderID renderID(Vector2I const& tilePosition);
    VisibilityID visibilityID(Vector2I const& tilePosition);
    bool isSolid(Vector2I const& tilePosition);
    bool blocksVision(Vector2I const& tilePosition);
};

#endif