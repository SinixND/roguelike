#ifndef IG20240426003633
#define IG20240426003633

#include "RenderId.h"
#include <raylib.h>
#include <string>
#include <unordered_map>

struct Textures
{
    std::unordered_map<RenderId, Vector2> textureIdToAtlasPosition{};

    //* Texture atlas
    Texture2D atlas{};
};

namespace TexturesModule
{
    void unloadAtlas( Textures const& textures );

    //* Texture atlas
    [[nodiscard]]
    Textures const& loadAtlas(
        Textures& textures,
        std::string const& filename
    );

    [[nodiscard]]
    Textures const& registerTexture(
        Textures& textures,
        RenderId textureId,
        Vector2 position
    );

    Vector2 const& getTexturePosition(
        Textures const& textures,
        RenderId renderId
    );
}
#endif
