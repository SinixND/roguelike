#ifndef IG20240531150939
#define IG20240531150939

#include <cstddef>
#include <raylib.h>

enum class RenderId;
struct Chunk;
struct Fog;
struct RenderData;
class Textures;

namespace RenderSystem
{
    void loadRenderData(RenderData& renderData);

    void render(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Color const& tint = WHITE);

    void renderToChunk(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Chunk& chunk,
        Color const& tint = WHITE);

    void renderChunk(Chunk const& chunk);

    void renderFog(Fog const& fog);

    void cycleThemes(size_t& theme);

    void deinit(Textures& textures);
}

#endif
