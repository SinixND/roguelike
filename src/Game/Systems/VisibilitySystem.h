#ifndef IG20240816170849
#define IG20240816170849

//* https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "raylibEx.h"
#include <unordered_set>
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

struct Fog
{
    Vector2I tilePosition{};
    bool isFogOpaque{};
};

enum class VisibilityId;

namespace VisibilitySystem
{
    void update(
        snx::DenseMap<Vector2I, Fog>& fogs_,
        snx::DenseMap<Vector2I, VisibilityId>& visibilityIds,
        std::unordered_set<Vector2I> const& isOpaques,
        RectangleExI const& viewport,
        int visionRange,
        Vector2I const& heroPosition);

    //* If any part of tile is visible . whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilitiesInOctant(
        snx::DenseMap<Vector2I, Fog>& fogs_,
        int octant,
        snx::DenseMap<Vector2I, VisibilityId>& visibilityIds,
        std::unordered_set<Vector2I> const& isOpaques,
        Vector2I const& heroPosition,
        int visionRange,
        int range);

    void updateShadowline(
        std::vector<Shadow>& shadowline,
        Vector2I const& octantPosition);
};

#endif
