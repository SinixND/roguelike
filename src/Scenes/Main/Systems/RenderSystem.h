#ifndef IG20240531150939
#define IG20240531150939

#include <cstddef>
#include <raylib.h>

enum class RenderId;
struct Chunks;
enum class Fog;
struct RenderData;
struct Textures;

namespace RenderSystem
{
    void loadRenderData(RenderData& renderData);

    void renderTile(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Color const& tint = WHITE);

    void renderToChunk(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Vector2 const& chunkPixel,
        Color const& tint = WHITE);

    void renderChunk(
        Texture const& chunkTexture,
        Vector2 const& worldPixel);

    void renderFog(
        Vector2 const& position,
        Fog fog);

    void cycleThemes(size_t& theme);

    void deinit(Textures& textures);
}

#endif
