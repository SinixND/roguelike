#ifndef IG20240602211712
#define IG20240602211712

#include "Position.h"
#include "RenderID.h"
#include "SparseSet.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <string>
#include <vector>

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

    int tileCount_{};
    RectangleExI mapSize_{};

public:
    void insert(
        Vector2I const& tilePosition,
        RenderID renderID,
        std::string const& tag,
        bool isSolid,
        bool blocksVision);

    void erase(Vector2I const& tilePosition);

    std::vector<Position>& position();
    Position& position(Vector2I const& tilePosition);

    std::vector<std::string> const& tag();
    std::string const& tag(Vector2I const& tilePosition);

    std::vector<RenderID> const& renderID();
    RenderID renderID(Vector2I const& tilePosition);

    std::vector<VisibilityID> const& visibilityID();
    VisibilityID visibilityID(Vector2I const& tilePosition);

    std::vector<bool> const& isSolid();
    bool isSolid(Vector2I const& tilePosition);

    std::vector<bool> const& blocksVision();
    bool blocksVision(Vector2I const& tilePosition);

    void updateMapSize(Vector2I const& tilePosition);
    int tileCount() { return tileCount_; }
};

#endif
