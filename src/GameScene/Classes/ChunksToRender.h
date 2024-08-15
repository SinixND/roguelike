#ifndef IG20240815193400
#define IG20240815193400

#include "Chunk.h"
#include "DenseMap.h"
#include "Renderer.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class ChunksToRender
{
    snx::DenseMap<Vector2I, Chunk> chunksToRender_{};

public:
    void init(Tiles& map, Renderer& renderer);

    // Verify chunk exists for tilePosition
    void verifyRequiredChunk(Vector2I const& tilePosition);
    RenderTexture& chunk(Vector2I const& tilePosition);
    std::vector<Chunk>& chunks();
};

#endif