#ifndef IG20240815193400
#define IG20240815193400

#include "Chunk.h"
#include "DenseMap.h"
#include "Renderer.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>

class Chunks
{
    snx::DenseMap<Vector2I, Chunk> chunks_{};

public:
    void init(Tiles& tiles, Renderer& renderer);

    // Verify chunk exists for tilePosition
    void verifyRequiredChunk(Vector2I const& tilePosition);
    RenderTexture& chunk(Vector2I const& tilePosition);
    snx::DenseMap<Vector2I, Chunk>& chunks();
};

#endif
