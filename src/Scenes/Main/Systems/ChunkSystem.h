#ifndef IG20240815193400
#define IG20240815193400

#include "DenseMap.h"

struct Vector2I;
struct Chunk;
struct Vector2;
enum class RenderId;
struct Textures;

namespace ChunkSystem
{
    void initializeChunks(
        Textures const& textures,
        snx::DenseMap<Vector2I, Chunk>& chunks,
        snx::DenseMap<Vector2I, Vector2> const& tilesPositions,
        snx::DenseMap<Vector2I, RenderId> const& tilesRenderIds);
};

#endif
