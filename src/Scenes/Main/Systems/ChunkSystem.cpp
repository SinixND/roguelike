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
    Vector2I chunkPosition{Convert::tileToChunk(tilePosition)};

    //* If clause is needed due to exclude unnecessary LoadRenderTexture() calls
    if (!chunks.contains(chunkPosition))
    {
        chunks.emplace(
            chunkPosition,
            PositionComponent{chunkPosition},
            LoadRenderTexture(
                ChunkData::chunkSize_f,
                ChunkData::chunkSize_f));
    }
}

void ChunkSystem::initializeChunks(
    Textures const& textures,
    snx::DenseMap<Vector2I, Chunk>& chunks,
    snx::DenseMap<Vector2I, PositionComponent> const& tilesPositions,
    snx::DenseMap<Vector2I, RenderId> const& tilesRenderIds)
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
            PositionModule::tilePosition(position),
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
                    tilesPositions.at(tilePosition).worldPixel,
                    chunk);
            }
        }

        EndTextureMode();
    }
}
