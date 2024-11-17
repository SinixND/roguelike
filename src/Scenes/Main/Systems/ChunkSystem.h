#ifndef IG20240815193400
#define IG20240815193400

namespace snx
{
    template <typename Key, typename Type>
    class DenseMap;
}

struct Vector2I;
struct Chunk;
struct Vector2;
enum class RenderId;
class Textures;

namespace ChunkSystem
{
    void initializeChunks(
        Textures const& textures,
        snx::DenseMap<Vector2I, Chunk>& chunks,
        snx::DenseMap<Vector2I, Vector2> const tilesPositions,
        snx::DenseMap<Vector2I, RenderId> const& tilesRenderIds);
};

#endif
