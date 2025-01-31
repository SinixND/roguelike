#ifndef IG20240816170849
#define IG20240816170849

//* https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "raylibEx.h"
#include <unordered_set>
#include <vector>

class Tiles;
enum class Fog;
enum class VisibilityId;

struct Shadow
{
    float slopeLeft{};
    float slopeRight{};

    explicit Shadow( Vector2I const& octantPosition );
};

namespace ShadowModule
{
    float slopeLeft( Shadow const& shadow );

    void setSlopeLeft(
        Shadow& shadow,
        Vector2I const& octantPosition
    );

    void setSlopeLeft(
        Shadow& shadow,
        float slopeLeft
    );

    float slopeRight( Shadow const& shadow );

    void setSlopeRight(
        Shadow& shadow,
        Vector2I const& octantPosition
    );

    void setSlopeRight(
        Shadow& shadow,
        float slopeRight
    );

    //* Get x at left slope for height of top-left octant tile corner
    float getLeftAtTop(
        Shadow const& shadow,
        Vector2I const& octantPosition
    );

    //* Get x at left slope for height of bottom-right octant tile corner
    float getLeftAtBottom(
        Shadow const& shadow,
        Vector2I const& octantPosition
    );

    //* Get x at left slope for height of octant position
    float getLeft(
        Shadow const& shadow,
        int octantPositionHeight
    );

    //* Get x at right slope for height of top-left octant tile corner
    float getRightAtTop(
        Shadow const& shadow,
        Vector2I const& octantPosition
    );

    //* Get x at right slope for height of bottom-right octant tile corner
    float getRightAtBottom(
        Shadow const& shadow,
        Vector2I const& octantPosition
    );

    //* Get x at right slope for height of octant position
    float getRight(
        Shadow const& shadow,
        int octantPositionHeight
    );
}

namespace VisibilitySystem
{
    //* If any part of tile is visible . whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilities(
        snx::DenseMap<Vector2I, Fog>& fogsIO,
        Tiles& tilesIO,
        RectangleExI const& viewportInTiles,
        Vector2I const& heroPosition,
        int visionRange
    );
};

#endif
