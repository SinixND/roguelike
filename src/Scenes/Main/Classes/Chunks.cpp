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

void Chunks::init(Tiles const& tiles, Renderer& renderer)
{
    // Reset
    for (Chunk const& chunk : chunks().values())
    {
        UnloadRenderTexture(chunk.renderTexture_);
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

        BeginTextureMode(chunk.renderTexture_);

        ClearBackground(BG_COLOR);

        for (int x{chunkSize.left() - 1}; x < (chunkSize.right() + 2); ++x)
        {
            for (int y{chunkSize.top() - 1}; y < (chunkSize.bottom() + 2); ++y)
            {
                Vector2I tilePosition{x, y};

                if (!tiles.renderIDs().contains(tilePosition))
                {
                    continue;
                }

                renderer.renderToChunk(
                    tiles.renderID(tilePosition),
                    tiles.position(tilePosition).worldPixel(),
                    chunk);
            }
        }

        EndTextureMode();
    }
}

void Chunks::verifyRequiredChunk(Vector2I const& tilePosition)
{
    Vector2I chunkPosition{UnitConversion::tileToChunk(tilePosition)};

    // If clause is needed due to exclude unnecessary LoadRenderTexture() calls
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

snx::DenseMap<Vector2I, Chunk> const& Chunks::chunks() const
{
    return chunks_;
}
