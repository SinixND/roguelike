#include "Chunks.h"

#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "DenseMap.h"
#include "Position.h"
#include "Renderer.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

void Chunks::init(Tiles& tiles, Renderer& renderer)
{
    // Reset
    for (Chunk& chunk : chunks().values())
    {
        UnloadRenderTexture(chunk.renderTexture());
    }

    chunks_.clear();

    // Create necessary chunks
    for (Position const& position : tiles.positions().values())
    {
        verifyRequiredChunk(position.tilePosition());
    }

    // Render tiles to chunk
    for (Chunk& chunk : chunks_.values())
    {
        RectangleExI const& chunkSize{chunk.corners()};

        BeginTextureMode(chunk.renderTexture());

        ClearBackground(BG_COLOR);

        for (int x{chunkSize.left() - 1}; x < (chunkSize.right() + 2); ++x)
        {
            for (int y{chunkSize.top() - 1}; y < (chunkSize.bottom() + 2); ++y)
            {
                Vector2I tilePosition{x, y};
                if (tilePosition == Vector2I{-8, 0})
                {
                    [[maybe_unused]] int dbg{};
                }
                // if (!tiles.renderIDs().contains(tilePosition))
                // {
                //     continue;
                // }

                renderer.renderToChunk(
                    tiles.renderID(tilePosition),
                    tiles.position(tilePosition).worldPosition(),
                    chunk);
            }
        }

        EndTextureMode();
    }
}

void Chunks::verifyRequiredChunk(Vector2I const& tilePosition)
{
    Vector2I chunkPosition{UnitConversion::tileToChunk(tilePosition)};

    // If clause needed due to LoadRenderTexture() call
    if (!chunks_.contains(chunkPosition))
    {
        chunks_.emplace(
            chunkPosition,
            LoadRenderTexture(
                ChunkData::CHUNK_SIZE_F,
                ChunkData::CHUNK_SIZE_F),
            Position{chunkPosition});
    }
}

RenderTexture& Chunks::chunk(Vector2I const& tilePosition)
{
    return chunks_[UnitConversion::tileToChunk(tilePosition)].renderTexture();
}

snx::DenseMap<Vector2I, Chunk>& Chunks::chunks()
{
    return chunks_;
}
