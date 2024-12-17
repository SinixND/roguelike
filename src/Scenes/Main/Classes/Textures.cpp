#include "Textures.h"
#include "RenderId.h"
#include <raylib.h>
#include <string>
#include <utility>

enum class RenderId;

void Textures::loadAtlas(std::string const& filename)
{
    unloadAtlas();
    textureAtlas_ = LoadTexture((texturePath + filename).c_str());
}

Texture2D const& Textures::textureAtlas() const
{
    return textureAtlas_;
}

void Textures::registerTexture(RenderId textureId, Vector2 position)
{
    textureIdToAtlasPosition_.insert({textureId, position});
}

Vector2 const& Textures::getTexturePosition(RenderId renderId) const
{
    return textureIdToAtlasPosition_.at(renderId);
}

void Textures::unloadAtlas()
{
    UnloadTexture(textureAtlas_);
}
