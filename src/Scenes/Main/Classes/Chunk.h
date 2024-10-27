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

    Chunk(RenderTexture const& renderTexture, PositionComponent const& position)
        : corners(
            RectangleExI{
                position.tilePosition().x,
                position.tilePosition().y,
                ChunkData::CHUNK_SIZE_I,
                ChunkData::CHUNK_SIZE_I})
        , position(position)
        , renderTexture(renderTexture)
    {
    }
};

#endif
