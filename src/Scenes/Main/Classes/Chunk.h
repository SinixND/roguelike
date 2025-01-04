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
struct Chunk
{
    RectangleExI corners{};
    PositionComponent position{};
    RenderTexture renderTexture{};

    Chunk() = default;

    Chunk(
        PositionComponent const& position,
        RenderTexture const& texture)
        : corners(
              RectangleExI{
                  PositionModule::tilePosition(position).x,
                  PositionModule::tilePosition(position).y,
                  ChunkData::chunkSize,
                  ChunkData::chunkSize})
        , position(position)
        , renderTexture(texture)
    {
    }
};

#endif
