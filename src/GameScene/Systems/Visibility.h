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
    Shadow(Vector2I const& octantPosition);

    float slopeLeft() const { return slopeLeft_; }
    void setSlopeLeft(Vector2I const& octantPosition);
    void setSlopeLeft(float slopeLeft);

    float slopeRight() const { return slopeRight_; }
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
    Vector2I const& tilePosition() const { return tilePosition_; }
    void setTilePosition(Vector2I const& tilePosition) { tilePosition_ = tilePosition; }

    bool isFogOpaque() const { return isFogOpaque_; }
    void setFogOpaque(bool isFogOpaque) { isFogOpaque_ = isFogOpaque; }

    Fog() = default;

    Fog(Vector2I const& tilePosition, bool isFogOpaque)
        : tilePosition_(tilePosition)
        , isFogOpaque_(isFogOpaque)
    {
    }
};

class Visibility
{
    snx::DenseMap<Vector2I, Fog> fogsToRender_{};

private:
    // If any part of tile is visible -> whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilitiesInOctant(
        int octant,
        Tiles& map,
        Vector2I const& heroPosition,
        int range);

    void updateShadowline(
        std::vector<Shadow>& shadowline,
        Vector2I const& octantPosition);

public:
    std::vector<Fog>& fogsToRender() { return fogsToRender_.values(); }

    void update(
        Tiles& map,
        RectangleExI const& viewport,
        Vector2I const& heroPosition);
};

#endif
