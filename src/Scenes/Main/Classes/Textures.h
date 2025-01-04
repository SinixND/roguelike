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
    //* Texture atlas
    void loadAtlas(
        Textures& textures,
        std::string const& filename);

    void registerTexture(
        Textures& textures,
        RenderId textureId,
        Vector2 position);

    Vector2 const& getTexturePosition(
        Textures const& textures,
        RenderId renderId);

    void unloadAtlas(
        Textures& textures);
}
#endif
