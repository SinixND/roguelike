#include "ChunksToRender.h"

#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "Position.h"
#include "Renderer.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

void ChunksToRender::init(Tiles& map, Renderer& renderer)
{
    // Reset
    for (Chunk& chunk : chunks())
    {
        UnloadRenderTexture(chunk.renderTexture());
    }

    chunksToRender_.clear();

    // Create necessary chunks
    for (auto const& position : map.positions().values())
    {
        verifyRequiredChunk(position.tilePosition());
    }

    // Render tiles to chunk
    for (Chunk& chunk : chunksToRender_.values())
    {
        RectangleExI const& chunkSize{chunk.corners()};

        BeginTextureMode(chunk.renderTexture());

        ClearBackground(BG_COLOR);

        for (int x{chunkSize.left()}; x <= chunkSize.right(); ++x)
        {
            for (int y{chunkSize.top()}; y <= chunkSize.bottom(); ++y)
            {
                Vector2I tilePosition{x, y};
                renderer.renderToChunk(
                    map.renderID(tilePosition),
                    map.position(tilePosition).worldPosition(),
                    chunk);
            }
        }

        EndTextureMode();
    }
}

void ChunksToRender::verifyRequiredChunk(Vector2I const& tilePosition)
{
    Vector2I chunkPosition{UnitConversion::tileToChunk(tilePosition)};

    // If clause needed due to LoadRenderTexture() call
    if (!chunksToRender_.contains(chunkPosition))
    {
    chunksToRender_.emplace(
        chunkPosition,
        LoadRenderTexture(
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F),
        Position{chunkPosition});
    }
}

RenderTexture& ChunksToRender::chunk(Vector2I const& tilePosition)
{
    verifyRequiredChunk(tilePosition);

    return chunksToRender_[tilePosition].renderTexture();
}

std::vector<Chunk>& ChunksToRender::chunks()
{
    return chunksToRender_.values();
}
