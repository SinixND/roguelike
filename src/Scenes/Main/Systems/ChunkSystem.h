#ifndef IG20240815193400
#define IG20240815193400

namespace snx
{
    template <typename Key, typename Type>
    class DenseMap;
}
class RenderSystem;
struct Vector2I;
class Chunk;
class PositionComponent;
enum class RenderID;

namespace ChunkSystem
{
    void initializeChunks(
        snx::DenseMap<Vector2I, Chunk>& chunks,
        snx::DenseMap<Vector2I, PositionComponent> const tilesPositions,
        snx::DenseMap<Vector2I, RenderID> const& tilesRenderIDs,
        RenderSystem& renderer);
};

#endif
