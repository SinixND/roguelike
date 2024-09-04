#ifndef IG20240904170921
#define IG20240904170921

#include "raylibEx.h"

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

#endif