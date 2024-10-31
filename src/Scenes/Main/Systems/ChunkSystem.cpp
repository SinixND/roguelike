#include "ChunkSystem.h"
#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "DenseMap.h"
#include "PositionComponent.h"
#include "RenderSystem.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

void verifyRequiredChunkExists(
    Vector2I const& tilePosition,
    snx::DenseMap<Vector2I, Chunk>& chunks)
{
    Vector2I chunkPosition{UnitConversion::tileToChunk(tilePosition)};

    //* If clause is needed due to exclude unnecessary LoadRenderTexture() calls
    if (!chunks.contains(chunkPosition))
    {
        chunks.emplace(
            chunkPosition,
            LoadRenderTexture(
                ChunkData::CHUNK_SIZE_F,
                ChunkData::CHUNK_SIZE_F),
            PositionComponent{chunkPosition});
    }
}

void ChunkSystem::initializeChunks(
    snx::DenseMap<Vector2I, Chunk>& chunks,
    snx::DenseMap<Vector2I, PositionComponent> const tilesPositions,
    snx::DenseMap<Vector2I, RenderID> const& tilesRenderIDs,
    RenderSystem& renderer)
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

        ClearBackground(BG_COLOR);

        for (int x{chunkSize.left() - 1}; x < (chunkSize.right() + 2); ++x)
        {
            for (int y{chunkSize.top() - 1}; y < (chunkSize.bottom() + 2); ++y)
            {
                Vector2I tilePosition{x, y};

                if (!tilesRenderIDs.contains(tilePosition))
                {
                    continue;
                }

                renderer.renderToChunk(
                    tilesRenderIDs.at(tilePosition),
                    tilesPositions.at(tilePosition).worldPixel(),
                    chunk);
            }
        }

        EndTextureMode();
    }
}
