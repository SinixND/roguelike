#ifndef IG20240816170849
#define IG20240816170849

//* https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "raylibEx.h"

struct Tiles;
enum class FogStateId;

namespace VisibilitySystem
{
    /// If any part of tile is visible => whole tile is visible (so that "tunnel walls" stay visible)
    /// Shadow slopes calculated from center of hero
    [[nodiscard]]
    Tiles const& calculateVisibilities(
        Tiles& tiles,
        snx::DenseMap<Vector2I, FogStateId>& fogsIO,
        RectangleExI const& viewportInTiles,
        Vector2I const& heroPosition,
        int visionRange
    );
};

#endif
