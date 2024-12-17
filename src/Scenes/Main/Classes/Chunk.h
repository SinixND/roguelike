#ifndef IG20240815194414
#define IG20240815194414

#include "ChunkData.h"
#include "PositionComponent.h"
#include "raylibEx.h"
#include <raylib.h>

//* Contains
//* - one renderTexture
//* - its corners as a RectangleExI (tile coordinates)
//* - center position
class Chunk
{
public:
    RectangleExI corners{};
    PositionComponent position{};
    RenderTexture renderTexture{};

public:
    Chunk() = default;

    Chunk(
        PositionComponent const& position,
        RenderTexture const& renderTexture)
        : corners(
              RectangleExI{
                  position.tilePosition().x,
                  position.tilePosition().y,
                  ChunkData::chunkSize,
                  ChunkData::chunkSize})
        , position(position)
        , renderTexture(renderTexture)
    {
    }
};

// Chunk newChunk(
//     PositionComponent const& position,
//     RenderTexture const& renderTexture)
// {
//     Chunk chunk{};
//
//     chunk.corners = RectangleExI{
//         position.tilePosition().x,
//         position.tilePosition().y,
//         ChunkData::chunkSize,
//         ChunkData::chunkSize};
//
//     chunk.position = position;
//
//     chunk.renderTexture = renderTexture;
//
//     return chunk;
// }
//
#endif
