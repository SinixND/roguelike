#ifndef IG20240312000918
#define IG20240312000918
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

namespace RayCast
{

    // Tiles passed by ray
    std::vector<Tile*> getTilesRayed(
      std::vector<Vector2i>& rayTargets,
      Vector2 origin,
      TileMap& tileMap);

}

#endif