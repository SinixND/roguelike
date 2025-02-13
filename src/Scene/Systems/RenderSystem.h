#ifndef IG20240531150939
#define IG20240531150939

#include <cstddef>
#include <raylib.h>

enum class RenderId;
struct Chunk;
enum class Fog;
struct RenderData;
struct Textures;

namespace RenderSystem
{
    [[nodiscard]]
    RenderData const& loadRenderData( RenderData& renderData );

    void renderTexture(
        Textures const& textures,
        Vector2 const& worldPixel,
        RenderId renderId,
        Color const& tint = WHITE
    );

    void renderChunk( Chunk const& chunk );

    void renderFog(
        Vector2 const& fogPosition,
        Fog fog
    );

    size_t cycleThemes( size_t theme );

    void deinit( Textures const& textures );
}

#endif
