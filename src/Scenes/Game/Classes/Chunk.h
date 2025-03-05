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
    RenderTexture renderTexture{};
    RectangleExI rectangle{};
    Vector2 position{};

    Chunk() = default;

    Chunk(
        Vector2 const& position,
        RenderTexture const& texture
    )
        : renderTexture( texture )
        , rectangle(
              RectangleExI{
                  Convert::worldToTile( position ).x,
                  Convert::worldToTile( position ).y,
                  ChunkData::chunkSize,
                  ChunkData::chunkSize
              }
          )
        , position( position )
    {
    }
};

#endif
