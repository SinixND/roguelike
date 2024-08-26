#ifndef IG20240816170849
#define IG20240816170849

// https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class Visibility
{
    class Shadow
    {
        float slopeLeft_{};
        float slopeRight_{};

    public:
        Shadow(Vector2I const& octantPosition);

        float slopeLeft() { return slopeLeft_; }
        void setSlopeLeft(Vector2I const& octantPosition);
        void setSlopeLeft(float slopeLeft);

        float slopeRight() { return slopeRight_; }
        void setSlopeRight(Vector2I const& octantPosition);
        void setSlopeRight(float slopeRigt);

        // Get x at left slope for height of top-left octant tile corner
        float getLeftAtTop(Vector2I const& octantPosition);
        // Get x at left slope for height of bottom-right octant tile corner
        float getLeftAtBottom(Vector2I const& octantPosition);
        // Get x at left slope for height of octant position
        float getLeft(int octantPositionHeight);

        // Get x at right slope for height of top-left octant tile corner
        float getRightAtTop(Vector2I const& octantPosition);
        // Get x at right slope for height of bottom-right octant tile corner
        float getRightAtBottom(Vector2I const& octantPosition);
        // Get x at right slope for height of octant position
        float getRight(int octantPositionHeight);
    };

private:
    // If any part of tile is visible -> whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilitiesInOctant(
        int octant,
        Tiles& map,
        Vector2I const& heroPosition,
        int range);

    void addShadow(
        std::vector<Shadow>& shadowline,
        Vector2I const& sectorPosition);

    // void calculateVisibilitiesInDiagonal( int quadXMin, int quadXMax, int quadYMin, int quadYMax, std::vector<Shadow>& shadowline, Tiles& map, int quarter, Vector2I const& heroPosition);

    // void calculateVisibilitiesInQuarter( int quarter, Tiles& map, Vector2I const& heroPosition, int width, int height);

public:
    void update(
        Tiles& map,
        RectangleEx const& mapPanel,
        Vector2I const& heroPosition);
};

#endif
