#ifndef IG20240815194414
#define IG20240815194414

#include "ChunkData.h"
#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

//* Contains
//* - one renderTexture
//* - its corners as a RectangleExI (tile coordinates)
//* - center position
struct Chunk
{
    RectangleExI corners{};
    Vector2 position{};
    RenderTexture renderTexture{};

    Chunk() = default;

    Chunk(
        Vector2 const& position,
        RenderTexture const& texture)
        : corners(
              RectangleExI{
                  Convert::worldToTile(position).x,
                  Convert::worldToTile(position).y,
                  ChunkData::chunkSize,
                  ChunkData::chunkSize})
        , position(position)
        , renderTexture(texture)
    {
    }
};

#endif
