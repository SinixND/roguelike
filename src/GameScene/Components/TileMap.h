#ifndef IG20240211173624
#define IG20240211173624

#include "GameObject.h"
#include "SparseSet.h"
#include "Tile.h"
#include "raylibEx.h"

using TileMap = snx::SparseSet<Vector2I, Tile>;
using TileOverlayMap = snx::SparseSet<Vector2I, GameObject>;

#endif