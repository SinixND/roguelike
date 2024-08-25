#ifndef IG20240531150939
#define IG20240531150939

#include "Chunk.h"
#include "RenderID.h"
#include "Textures.h"
#include <raylib.h>

class Renderer
{
    Textures textures_{};

public:
    void init();
    void render(RenderID renderID, Vector2 const& worldPosition, Color const& tint = WHITE);
    void renderChunk(Chunk& chunk);
    void renderToChunk(RenderID renderID, Vector2 const& worldPosition, Chunk& chunk, Color const& tint = WHITE);
    void deinit();
};

#endif
