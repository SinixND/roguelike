#ifndef IG20240815193400
#define IG20240815193400

#include "DenseMap.h"

struct Vector2I;
struct Chunk;
struct Textures;
class Tiles;

namespace ChunkSystem
{
    [[nodiscard]]
    snx::DenseMap<Vector2I, Chunk> const& reRenderChunks(
        snx::DenseMap<Vector2I, Chunk>& chunks,
        Textures const& textures,
        Tiles const& tiles
    );
};

#endif
