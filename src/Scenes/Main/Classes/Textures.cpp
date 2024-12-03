#include "Textures.h"
#include "RenderID.h"
#include <raylib.h>
#include <string>
#include <utility>

enum class RenderID;

void Textures::loadAtlas(std::string const& filename)
{
    unloadAtlas();
    textureAtlas_ = LoadTexture((texturePath + filename).c_str());
}

Texture2D const& Textures::textureAtlas() const
{
    return textureAtlas_;
}

void Textures::registerTexture(RenderID textureID, Vector2 position)
{
    textureIDToAtlasPosition_.insert(std::make_pair(textureID, position));
}

Vector2 const& Textures::getTexturePosition(RenderID renderID) const
{
    return textureIDToAtlasPosition_.at(renderID);
}

void Textures::unloadAtlas()
{
    UnloadTexture(textureAtlas_);
}
