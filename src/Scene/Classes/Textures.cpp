#include "Textures.h"
#include "RenderId.h"
#include "TextureData.h"
#include <raylib.h>
#include <string>

enum class RenderId;

namespace TexturesModule
{
    void loadAtlas(
        Textures& textures,
        std::string const& filename
    )
    {
        TexturesModule::unloadAtlas( textures );
        textures.atlas = LoadTexture( ( TextureData::texturePath + filename ).c_str() );
    }

    void registerTexture(
        Textures& textures,
        RenderId textureId,
        Vector2 position
    )
    {
        textures.textureIdToAtlasPosition.insert( { textureId, position } );
    }

    Vector2 const& getTexturePosition(
        Textures const& textures,
        RenderId renderId
    )
    {
        return textures.textureIdToAtlasPosition.at( renderId );
    }

    void unloadAtlas(
        Textures& textures
    )
    {
        UnloadTexture( textures.atlas );
    }
}
