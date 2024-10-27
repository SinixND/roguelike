#ifndef IG20240815193400
#define IG20240815193400

#include "Chunk.h"
#include "DenseMap.h"
#include "RenderSystem.h"
#include "raylibEx.h"
#include <raylib.h>

namespace ChunkSystem
{
    void init(
        snx::DenseMap<Vector2I, Chunk>& chunks,
        snx::DenseMap<Vector2I, PositionComponent> const tilesPositions,
        snx::DenseMap<Vector2I, RenderID> const& tilesRenderIDs,
        RenderSystem& renderer);
};

#endif
