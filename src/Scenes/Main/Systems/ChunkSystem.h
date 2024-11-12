#ifndef IG20240815193400
#define IG20240815193400

namespace snx
{
    template <typename Key, typename Type>
    class DenseMap;
}

struct Vector2I;
class Chunk;
class PositionComponent;
enum class RenderID;
class Textures;

namespace ChunkSystem
{
    void initializeChunks(
        Textures const& textures,
        snx::DenseMap<Vector2I, Chunk>& chunks,
        snx::DenseMap<Vector2I, PositionComponent> const tilesPositions,
        snx::DenseMap<Vector2I, RenderID> const& tilesRenderIDs);
};

#endif
