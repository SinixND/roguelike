#ifndef IG20240816170849
#define IG20240816170849

//* https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <unordered_set>
#include <vector>

struct Tiles;

class Shadow
{
    float slopeLeft_{};
    float slopeRight_{};

public:
    explicit Shadow(Vector2I const& octantPosition);

    float slopeLeft() const;
    void setSlopeLeft(Vector2I const& octantPosition);
    void setSlopeLeft(float slopeLeft);

    float slopeRight() const;
    void setSlopeRight(Vector2I const& octantPosition);
    void setSlopeRight(float slopeRight);

    //* Get x at left slope for height of top-left octant tile corner
    float getLeftAtTop(Vector2I const& octantPosition) const;
    //* Get x at left slope for height of bottom-right octant tile corner
    float getLeftAtBottom(Vector2I const& octantPosition) const;
    //* Get x at left slope for height of octant position
    float getLeft(int octantPositionHeight) const;

    //* Get x at right slope for height of top-left octant tile corner
    float getRightAtTop(Vector2I const& octantPosition) const;
    //* Get x at right slope for height of bottom-right octant tile corner
    float getRightAtBottom(Vector2I const& octantPosition) const;
    //* Get x at right slope for height of octant position
    float getRight(int octantPositionHeight) const;
};

class Fog
{
public:
    Vector2I tilePosition{};
    bool isFogOpaque{};

public:
    Fog() = default;

    Fog(Vector2I const& tilePosition, bool isFogOpaque)
        : tilePosition(tilePosition)
        , isFogOpaque(isFogOpaque)
    {
    }
};

namespace VisibilitySystem
{
    void update(
        snx::DenseMap<Vector2I, Fog>& fogs_,
        snx::DenseMap<Vector2I, VisibilityID>& visibilityIDs,
        std::unordered_set<Vector2I> const& isOpaques,
        RectangleExI const& viewport,
        Vector2I const& heroPosition);

    //* If any part of tile is visible -> whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilitiesInOctant(
        snx::DenseMap<Vector2I, Fog>& fogs_,
        int octant,
        snx::DenseMap<Vector2I, VisibilityID>& visibilityIDs,
        std::unordered_set<Vector2I> const& isOpaques,
        Vector2I const& heroPosition,
        int range);

    void updateShadowline(
        std::vector<Shadow>& shadowline,
        Vector2I const& octantPosition);
};

#endif
