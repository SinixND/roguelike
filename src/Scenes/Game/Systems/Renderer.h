#ifndef IG20240531150939
#define IG20240531150939

#include "Chunk.h"
#include "RenderID.h"
#include "Textures.h"
#include "Visibility.h"
#include <raylib.h>

class Renderer
{
    Textures textures_{};

public:
    void init();

    void render(
        RenderID renderID,
        Vector2 const& worldPixel,
        Color const& tint = WHITE);

    void renderToChunk(
        RenderID renderID,
        Vector2 const& worldPixel,
        Chunk& chunk,
        Color const& tint = WHITE);

    void renderChunk(Chunk& chunk);

    void renderFog(Fog const& fog);

    void deinit();
};

#endif
