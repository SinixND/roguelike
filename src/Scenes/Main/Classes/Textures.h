#ifndef IG20240426003633
#define IG20240426003633

#include "RenderId.h"
#include <raylib.h>
#include <string>
#include <unordered_map>

class Textures
{
    std::string const texturePath{"resources/textures/"};

    //* Texture atlas
    Texture2D textureAtlas_{};
    std::unordered_map<RenderId, Vector2> textureIdToAtlasPosition_{};

public:
    //* Texture atlas
    void loadAtlas(std::string const& filename);

    void registerTexture(RenderId textureId, Vector2 position);

    Vector2 const& getTexturePosition(RenderId renderId) const;

    void unloadAtlas();

public:
    Texture2D const& textureAtlas() const;
};

#endif
