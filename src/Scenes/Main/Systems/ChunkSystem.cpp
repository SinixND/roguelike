#include "ChunkSystem.h"

#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "Convert.h"
#include "DenseMap.h"
#include "RenderSystem.h"
#include "raylibEx.h"
#include <raylib.h>

void verifyRequiredChunkExists(
    Vector2I const& tilePosition,
    snx::DenseMap<Vector2I, Chunk>& chunks)
{
    Vector2I chunkPosition{Convert::tileToChunk(tilePosition)};

    //* If clause is needed due to avoid unnecessary LoadRenderTexture() calls
    if (!chunks.contains(chunkPosition))
    {
        chunks.insert(
            chunkPosition,
            createChunk(
                Convert::tileToWorld(chunkPosition),
                LoadRenderTexture(
                    ChunkData::chunkSize_f,
                    ChunkData::chunkSize_f)));

        // chunks.emplace(
        //     chunkPosition,
        //     LoadRenderTexture(
        //         ChunkData::chunkSize_f,
        //         ChunkData::chunkSize_f),
        //     Convert::tileToWorld(chunkPosition));
    }
}

void ChunkSystem::initializeChunks(
    Textures const& textures,
    snx::DenseMap<Vector2I, Chunk>& chunks,
    snx::DenseMap<Vector2I, Vector2> const tilesPositions,
    snx::DenseMap<Vector2I, RenderId> const& tilesRenderIds)
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
            Convert::worldToTile(position),
            chunks);
    }

    //* Render to chunk
    for (Chunk& chunk : chunks)
    {
        RectangleExI const& chunkSize{chunk.corners};

        BeginTextureMode(chunk.renderTexture);

        ClearBackground(Colors::bg);

        for (int x{chunkSize.left() - 1}; x < (chunkSize.right() + 2); ++x)
        {
            for (int y{chunkSize.top() - 1}; y < (chunkSize.bottom() + 2); ++y)
            {
                Vector2I tilePosition{x, y};

                if (!tilesRenderIds.contains(tilePosition))
                {
                    continue;
                }

                RenderSystem::renderToChunk(
                    textures,
                    tilesRenderIds.at(tilePosition),
                    tilesPositions.at(tilePosition),
                    chunk);
            }
        }

        EndTextureMode();
    }
}
