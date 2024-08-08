#ifndef IG20240808122210
#define IG20240808122210

#include "TilesToRender.h"
#include "raylibEx.h"
#include "Tiles.h"

namespace TileFilters
{
    void initTilesToRender(
        TilesToRender& tilesToRender,
        Tiles& currentMap,
        RectangleExI const& renderRectangle);

    // void updateTilesToRender();
}

#endif
