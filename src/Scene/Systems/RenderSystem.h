#ifndef IG20240531150939
#define IG20240531150939

#include <cstddef>
#include <raylib.h>

enum class RenderId;
struct Chunk;
enum class Fog;
struct RenderData;
struct Textures;
struct Vector2I;

namespace RenderSystem
{
    void loadRenderData( RenderData& renderData );

    void renderTile(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Color const& tint = WHITE
    );

    void renderToChunk(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Chunk& chunk,
        Color const& tint = WHITE
    );

    void renderChunk( Chunk const& chunk );

    void renderFog(
        Vector2 const& fogPosition,
        Fog fog
    );

    void cycleThemes( size_t& theme );

    void deinit( Textures& textures );
}

#endif
