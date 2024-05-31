#ifndef IG20240531150939
#define IG20240531150939

#include "RenderID.h"
#include "Textures.h"
#include <raylib.h>

class Renderer
{
    Textures textures_{};

public:
    void init();
    void render(RenderID renderID, Vector2 const& renderPosition, Color const& tint = WHITE);
    void deinit();
};

#endif