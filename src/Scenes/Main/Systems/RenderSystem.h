#ifndef IG20240531150939
#define IG20240531150939

#include "Textures.h"
#include <cstddef>
#include <raylib.h>
#include <string>

enum class RenderID;
class Chunk;
struct Fog;

class RenderSystem
{
    Textures textures_{};

    //* Themes: TextureData::themes
    size_t theme_{1};

public:
    void init();

    void render(
        RenderID renderID,
        Vector2 const& worldPixel,
        Color const& tint = WHITE) const;

    void renderToChunk(
        RenderID renderID,
        Vector2 const& worldPixel,
        Chunk& chunk,
        Color const& tint = WHITE) const;

    void renderChunk(Chunk const& chunk) const;

    void renderFog(Fog const& fog) const;

    void cycleThemes();

    void deinit();

private:
    std::string textureAtlasFileName() const;
};

#endif
