#ifndef IG20240815193400
#define IG20240815193400

#include "Chunk.h"
#include "DenseMap.h"
#include "RenderSystem.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>

class Chunks
{
public:
    snx::DenseMap<Vector2I, Chunk> chunks{};

public:
    void init(Tiles const& tiles, RenderSystem& renderer);

private:
    //* Verify chunk exists for tilePosition
    void verifyRequiredChunk(Vector2I const& tilePosition);
};

#endif
