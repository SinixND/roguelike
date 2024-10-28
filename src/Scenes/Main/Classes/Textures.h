#ifndef IG20240426003633
#define IG20240426003633

#include "RenderID.h"
#include <raylib.h>
#include <string>
#include <unordered_map>

class Textures
{
    std::string const TEXTURE_PATH{"resources/textures/"};

    //* Texture atlas
    Texture2D textureAtlas_{};
    std::unordered_map<RenderID, Vector2> textureIdToAtlasPosition_{};

public:
    //* Texture atlas
    void loadAtlas(std::string const& filename);

    void registerTexture(RenderID textureID, Vector2 position);

    Vector2 const& getTexturePosition(RenderID renderID) const;

    void unloadAtlas();

public:
    Texture2D const& textureAtlas() const;
};

#endif
