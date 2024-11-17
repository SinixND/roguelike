#ifndef IG20240815194414
#define IG20240815194414

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
};

Chunk createChunk(
    Vector2 const& position,
    RenderTexture const& renderTexture);

#endif
