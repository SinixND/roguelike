#include "Chunk.h"

#include "ChunkData.h"
#include "Convert.h"

Chunk createChunk(
    Vector2 const& position,
    RenderTexture const& renderTexture)
{
    return Chunk{
        RectangleExI{
            Convert::worldToTile(position).x,
            Convert::worldToTile(position).y,
            ChunkData::chunkSize,
            ChunkData::chunkSize},
        position,
        renderTexture};
}

