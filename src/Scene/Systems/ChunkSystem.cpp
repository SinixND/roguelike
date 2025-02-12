#include "ChunkSystem.h"

#include "Chunk.h"
#include "ChunkData.h"
#include "Colors.h"
#include "Convert.h"
#include "DenseMap.h"
#include "RenderSystem.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>

[[nodiscard]]
snx::DenseMap<Vector2I, Chunk> const& requireChunk(
    snx::DenseMap<Vector2I, Chunk>& chunks,
    Vector2I const& tilePosition
)
{
    Vector2I chunkPosition{ Convert::tileToChunk( tilePosition ) };

    //* NOTE: !!! If clause is needed due to exclude unnecessary LoadRenderTexture() calls !!!
    if ( !chunks.contains( chunkPosition ) )
    {
        chunks.emplace(
            chunkPosition,
            Convert::tileToWorld( chunkPosition ),
            LoadRenderTexture(
                ChunkData::chunkSize_f,
                ChunkData::chunkSize_f
            )
        );
    }

    return chunks;
}

[[nodiscard]]
snx::DenseMap<Vector2I, Chunk> const& clearChunks( snx::DenseMap<Vector2I, Chunk>& chunks )
{
    for ( Chunk const& chunk : chunks )
    {
        UnloadRenderTexture( chunk.renderTexture );
    }

    chunks.clear();

    return chunks;
}

namespace ChunkSystem
{
    [[nodiscard]]
    snx::DenseMap<Vector2I, Chunk> const& renderChunks(
        snx::DenseMap<Vector2I, Chunk>& chunks,
        Textures const& textures,
        Tiles const& tiles
    )
    {
        chunks = clearChunks( chunks );

        //* Create necessary chunks
        for ( Vector2 const& position : tiles.positions )
        {
            chunks = requireChunk(
                chunks,
                Convert::worldToTile( position )
            );
        }

        //* Render to chunk
        for ( Chunk& chunk : chunks )
        {
            RectangleExI const& chunkSize{ chunk.rectangle };

            BeginTextureMode( chunk.renderTexture );

            ClearBackground( Colors::bg );

            //* Iterate all tilePositions in chunk
            for ( int x{ chunkSize.left() - 1 }; x < ( chunkSize.right() + 2 ); ++x )
            {
                for ( int y{ chunkSize.top() - 1 }; y < ( chunkSize.bottom() + 2 ); ++y )
                {
                    Vector2I tilePosition{ x, y };

                    if ( !tiles.ids.contains( tilePosition ) )
                    {
                        continue;
                    }

                    size_t tileId{ tiles.ids.at( tilePosition ) };

                    RenderSystem::renderToChunk(
                        textures,
                        tiles.renderIds.at( tileId ),
                        tiles.positions.at( tileId ),
                        chunk
                    );
                }
            }

            EndTextureMode();
        }

        return chunks;
    }
}
