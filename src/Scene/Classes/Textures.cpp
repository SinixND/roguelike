#include "Textures.h"
#include "RenderId.h"
#include "TextureData.h"
#include <raylib.h>
#include <string>

enum class RenderId;

namespace TexturesModule
{

    [[nodiscard]]
    Textures const& unloadAtlas(
        Textures& textures
    )
    {
        UnloadTexture( textures.atlas );

        return textures;
    }

    [[nodiscard]]
    Textures const& loadAtlas(
        Textures& textures,
        std::string const& filename
    )
    {
        textures = TexturesModule::unloadAtlas( textures );
        textures.atlas = LoadTexture( ( TextureData::texturePath + filename ).c_str() );

        return textures;
    }

    [[nodiscard]]
    Textures const& registerTexture(
        Textures& textures,
        RenderId textureId,
        Vector2 position
    )
    {
        textures.textureIdToAtlasPosition.insert( { textureId, position } );

        return textures;
    }

    Vector2 const& getTexturePosition(
        Textures const& textures,
        RenderId renderId
    )
    {
        return textures.textureIdToAtlasPosition.at( renderId );
    }
}
