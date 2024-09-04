#ifndef IG20240815194414
#define IG20240815194414

#include "ChunkData.h"
#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>

class Chunk
{
    RenderTexture renderTexture_{};
    RectangleExI corners_{};
    Position position_{};

public:
    RenderTexture& renderTexture() { return renderTexture_; }
    RectangleExI const& corners() { return corners_; }
    Position const& position() { return position_; }

    Chunk() = default;

    Chunk(RenderTexture const& renderTexture, Position const& position)
        : renderTexture_(renderTexture)
        , corners_(RectangleExI{
              position.tilePosition(),
              ChunkData::CHUNK_SIZE_I,
              ChunkData::CHUNK_SIZE_I})
        , position_(position)
    {
    }
};

#endif
