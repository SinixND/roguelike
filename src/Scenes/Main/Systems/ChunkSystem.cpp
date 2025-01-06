#include "ChunkSystem.h"

#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "Convert.h"
#include "RenderSystem.h"
#include "SparseSet.h"
#include "raylibEx.h"
#include <raylib.h>

void verifyRequiredChunkExists(
    snx::DenseMap<Vector2I, Chunk>& chunks,
    Vector2I const& tilePosition)
{
    Vector2I chunkPosition{Convert::tileToChunk(tilePosition)};

    //* If clause is needed due to exclude unnecessary LoadRenderTexture() calls
    if (!chunks.contains(chunkPosition))
    {
        chunks.emplace(
            chunkPosition,
            Convert::tileToWorld(chunkPosition),
            LoadRenderTexture(
                ChunkData::chunkSize_f,
                ChunkData::chunkSize_f));
    }
}

void ChunkSystem::init(
    Textures const& textures,
    snx::DenseMap<Vector2I, Chunk>& chunks,
    snx::DenseMap<Vector2I, size_t> const& tileIds,
    snx::SparseSet<Vector2> const& tilesPositions,
    snx::SparseSet<RenderId> const& tilesRenderIds)
{
    //* Reset
    for (Chunk const& chunk : chunks)
    {
        UnloadRenderTexture(chunk.renderTexture);
    }

    chunks.clear();

    //* Create necessary chunks
    for (Vector2 const& position : tilesPositions)
    {
        verifyRequiredChunkExists(
            chunks,
            Convert::worldToTile(position));
    }

    //* Render to chunk
    for (Chunk& chunk : chunks)
    {
        RectangleExI const& chunkSize{chunk.corners};

        BeginTextureMode(chunk.renderTexture);

        ClearBackground(Colors::bg);

        //* Iterate all tilePositions in chunk
        for (int x{chunkSize.left() - 1}; x < (chunkSize.right() + 2); ++x)
        {
            for (int y{chunkSize.top() - 1}; y < (chunkSize.bottom() + 2); ++y)
            {
                Vector2I tilePosition{x, y};

                if (!tileIds.contains(tilePosition))
                {
                    continue;
                }

                size_t tileId{tileIds.at(tilePosition)};

                RenderSystem::renderToChunk(
                    textures,
                    tilesRenderIds.at(tileId),
                    tilesPositions.at(tileId),
                    chunk.position);
            }
        }

        EndTextureMode();
    }
}
