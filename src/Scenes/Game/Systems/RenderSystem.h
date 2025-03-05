#ifndef IG20240531150939
#define IG20240531150939

#include <cstddef>
#include <raylib.h>

enum class RenderId;
struct Chunk;
enum class Fog;
struct RenderData;
struct Textures;
class Panel;
struct Hero;
class Objects;
struct Vector2I;

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

    void renderStatusPanel(
        Panel const& statusPanel,
        Hero const& hero,
        int mapLevel,
        Color const& bgColor,
        float borderWidth
    );

    void renderInfoPanel(
        Panel const& infoPanel,
        Objects const& objects,
        Vector2I const& cursorPosition,
        Color const& bgColor,
        float borderWidth
    );

    void renderLogPanel(
        Panel const& logPanel,
        Color const& bgColor,
        float borderWidth
    );

    [[nodiscard]]
    size_t cycleThemes( size_t theme );

    void deinit( Textures const& textures );
}

#endif
