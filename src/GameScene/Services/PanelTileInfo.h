#ifndef IG20240407004107
#define IG20240407004107

#include "TileMap.h"
#include "raylibEx.h"
#include <raylib.h>

namespace PanelTileInfo
{
    void update(TileMap& tileMap, Vector2I cursorPosition, Font const& font, RectangleEx const& panel);
}

#endif
