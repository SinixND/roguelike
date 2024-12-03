#include "ChunkSystem.h"
#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "Convert.h"
#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderSystem.h"
#include "raylibEx.h"
#include <raylib.h>

void verifyRequiredChunkExists(
    Vector2I const& tilePosition,
    snx::DenseMap<Vector2I, Chunk>& chunks)
{
    PositionComponent chunkPosition{tilePosition};

    //* If clause is needed due to exclude unnecessary LoadRenderTexture() calls
    if (!chunks.contains(chunkPosition.tilePosition()))
    {
        chunks.emplace(
            chunkPosition.tilePosition(),
            chunkPosition,
            LoadRenderTexture(
                ChunkData::chunkSize_f,
                ChunkData::chunkSize_f));
    }
}

void ChunkSystem::initializeChunks(
    Textures const& textures,
    snx::DenseMap<Vector2I, Chunk>& chunks,
    snx::DenseMap<Vector2I, PositionComponent> const tilesPositions,
    snx::DenseMap<Vector2I, RenderID> const& tilesRenderIDs)
{
    //* Reset
    for (Chunk const& chunk : chunks)
    {
        UnloadRenderTexture(chunk.renderTexture);
    }

    chunks.clear();

    //* Create necessary chunks
    for (PositionComponent const& position : tilesPositions)
    {
        verifyRequiredChunkExists(
            position.tilePosition(),
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

                if (!tilesRenderIDs.contains(tilePosition))
                {
                    continue;
                }

                RenderSystem::renderToChunk(
                    textures,
                    tilesRenderIDs.at(tilePosition),
                    tilesPositions.at(tilePosition).worldPixel(),
                    chunk);
            }
        }

        EndTextureMode();
    }
}
