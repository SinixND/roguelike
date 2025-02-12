#ifndef IG20240426003633
#define IG20240426003633

#include "RenderId.h"
#include <raylib.h>
#include <string>
#include <unordered_map>

struct Textures
{
    //* Texture atlas
    Texture2D atlas{};
    std::unordered_map<RenderId, Vector2> textureIdToAtlasPosition{};
};

namespace TexturesModule
{
    [[nodiscard]]
    Textures const& unloadAtlas( Textures& textures );
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
