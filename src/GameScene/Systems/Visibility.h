#ifndef IG20240816170849
#define IG20240816170849

// https://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include "DenseMap.h"
#include "Fog.h"
#include "Shadow.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

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

    void addShadow(
        std::vector<Shadow>& shadowline,
        Vector2I const& sectorPosition);

public:
    std::vector<Fog>& fogsToRender() { return fogsToRender_.values(); }

    void update(
        Tiles& map,
        RectangleEx const& viewport,
        Vector2I const& heroPosition);
};

#endif
