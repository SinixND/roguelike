#ifndef IG20240816170849
#define IG20240816170849

//* https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "SparseSet.h"
#include "raylibEx.h"
#include <unordered_set>
#include <vector>

struct Shadow
{
    float slopeLeft{};
    float slopeRight{};

    explicit Shadow(Vector2I const& octantPosition);
};

namespace ShadowModule
{
    float slopeLeft(Shadow const& shadow);

    void setSlopeLeft(
        Shadow& shadow,
        Vector2I const& octantPosition);

    void setSlopeLeft(
        Shadow& shadow,
        float slopeLeft);

    float slopeRight(Shadow const& shadow);

    void setSlopeRight(
        Shadow& shadow,
        Vector2I const& octantPosition);

    void setSlopeRight(
        Shadow& shadow,
        float slopeRight);

    //* Get x at left slope for height of top-left octant tile corner
    float getLeftAtTop(
        Shadow const& shadow,
        Vector2I const& octantPosition);

    //* Get x at left slope for height of bottom-right octant tile corner
    float getLeftAtBottom(
        Shadow const& shadow,
        Vector2I const& octantPosition);

    //* Get x at left slope for height of octant position
    float getLeft(
        Shadow const& shadow,
        int octantPositionHeight);

    //* Get x at right slope for height of top-left octant tile corner
    float getRightAtTop(
        Shadow const& shadow,
        Vector2I const& octantPosition);

    //* Get x at right slope for height of bottom-right octant tile corner
    float getRightAtBottom(
        Shadow const& shadow,
        Vector2I const& octantPosition);

    //* Get x at right slope for height of octant position
    float getRight(
        Shadow const& shadow,
        int octantPositionHeight);
}

enum class Fog
{
    transparent,
    opaque,
};

enum class VisibilityId;

namespace VisibilitySystem
{
    void update(
        snx::DenseMap<Vector2I, Fog>& fogs,
        snx::SparseSet<VisibilityId>& visibilityIds,
        snx::DenseMap<Vector2I, size_t> const& ids,
        std::unordered_set<size_t> const& isOpaques,
        RectangleExI const& viewport,
        int visionRange,
        Vector2I const& heroPosition);

    //* If any part of tile is visible . whole tile is visible (so that "tunnel walls" stay visible)
    void calculateVisibilitiesInOctant(
        snx::DenseMap<Vector2I, Fog>& fogs,
        snx::SparseSet<VisibilityId>& visibilityIds,
        snx::DenseMap<Vector2I, size_t> const& ids,
        std::unordered_set<size_t> const& isOpaques,
        Vector2I const& heroPosition,
        int octant,
        int visionRange,
        int range);

    void updateShadowline(
        std::vector<Shadow>& shadowline,
        Vector2I const& octantPosition);
};

#endif
