#ifndef IG20240815194414
#define IG20240815194414

#include "ChunkData.h"
#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>

// Contains
// - one renderTexture
// - its dimensions as a RectangleExI (tile coordinates)
// - center position

class Chunk
{
    RectangleExI corners_{};
    Position position_{};

public:
    RenderTexture renderTexture_{};

public:
    RectangleExI const& corners() const
    {
        return corners_;
    }

    Position const& position() const
    {
        return position_;
    }

    Chunk() = default;

    Chunk(RenderTexture const& renderTexture, Position const& position)
        : corners_(RectangleExI{
              position.tilePosition().x,
              position.tilePosition().y,
              ChunkData::CHUNK_SIZE_I,
              ChunkData::CHUNK_SIZE_I})
        , position_(position)
        , renderTexture_(renderTexture)
    {
    }
};

#endif
