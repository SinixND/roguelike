#ifndef IG20240426003633
#define IG20240426003633

#include "RenderID.h"
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <utility>

class Textures
{
private:
    std::string const texturePath{"resources/textures/"};

    // Texture atlas
    Texture2D textureAtlas_{};
    std::unordered_map<RenderID, Vector2> textureIdToAtlasPosition_{};

public:
    // Texture atlas
    void loadAtlas(std::string const& filename)
    {
        textureAtlas_ = LoadTexture((texturePath + filename).c_str());
    }

    Texture2D& getTextureAtlas()
    {
        return textureAtlas_;
    }

    void registerTexture(RenderID textureID, Vector2 position)
    {
        textureIdToAtlasPosition_.insert(std::make_pair(textureID, position));
    }

    Vector2& getTexturePosition(RenderID textureID)
    {
        return textureIdToAtlasPosition_.at(textureID);
    }

    void unloadAtlas()
    {
        UnloadTexture(textureAtlas_);
    }
};

#endif
