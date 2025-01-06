#ifndef IG20240815193400
#define IG20240815193400

#include "DenseMap.h"
#include "SparseSet.h"

struct Vector2I;
struct Chunk;
struct Vector2;
enum class RenderId;
struct Textures;

namespace ChunkSystem
{
    void init(
        Textures const& textures,
        snx::DenseMap<Vector2I, Chunk>& chunks,
        snx::DenseMap<Vector2I, size_t> const& ids,
        snx::SparseSet<Vector2> const& tilesPositions,
        snx::SparseSet<RenderId> const& tilesRenderIds);
};

#endif
