#ifndef IG20240816170849
#define IG20240816170849

// https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

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

    // Get x at left slope for height of top-left octant tile corner
    float getLeftAtTop(Vector2I const& octantPosition) const;
    // Get x at left slope for height of bottom-right octant tile corner
    float getLeftAtBottom(Vector2I const& octantPosition) const;
    // Get x at left slope for height of octant position
    float getLeft(int octantPositionHeight) const;

    // Get x at right slope for height of top-left octant tile corner
    float getRightAtTop(Vector2I const& octantPosition) const;
    // Get x at right slope for height of bottom-right octant tile corner
    float getRightAtBottom(Vector2I const& octantPosition) const;
    // Get x at right slope for height of octant position
    float getRight(int octantPositionHeight) const;
};

class Fog
{
    Vector2I tilePosition_{};
    bool isFogOpaque_{};

public:
    Fog() = default;

    Fog(Vector2I const& tilePosition, bool isFogOpaque)
        : tilePosition_(tilePosition)
        , isFogOpaque_(isFogOpaque)
    {
    }

    Vector2I const& tilePosition() const;

    bool isFogOpaque() const;
};

class Visibility
{
    snx::DenseMap<Vector2I, Fog> fogs_{};

public:
    void update(
        Tiles& tiles,
        RectangleExI const& viewport,
        Vector2I const& heroPosition);

    snx::DenseMap<Vector2I, Fog> const& fogs() const;

private:
    // If any part of tile is visible -> whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilitiesInOctant(
        int octant,
        Tiles& tiles,
        Vector2I const& heroPosition,
        int range);

    void updateShadowline(
        std::vector<Shadow>& shadowline,
        Vector2I const& octantPosition);
};

#endif
