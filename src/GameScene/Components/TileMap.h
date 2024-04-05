#ifndef IG20240211173624
#define IG20240211173624

#include "Entity.h"
#include "SparseSet.h"
#include "Tile.h"
#include "raylibEx.h"

using TileMap = snd::SparseSet<Vector2I, Tile>;
using TileOverlayMap = snd::SparseSet<Vector2I, Entity>;

#endif