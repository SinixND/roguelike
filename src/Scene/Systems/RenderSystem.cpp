#include "RenderSystem.h"
#include "Data/TextureData.h"
#include "Fog.h"
#include "RenderData.h"
#include "raylibEx.h"

//* #define DEBUG_CHUNKS
#define DEBUG_VISIBILITY

#include "Chunk.h"
#include "ChunkData.h"
#include "Convert.h"
#include "RenderId.h"
#include "TextureData.h"
#include "Textures.h"
#include "TileData.h"
#include "VisibilitySystem.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <string>

std::string textureAtlasFileName( size_t theme )
{
    return TextureData::themes[theme] + ".png";
}

namespace RenderSystem
{
    void loadRenderData( RenderData& renderData )
    {
        //* Load texture atlas
        TexturesModule::loadAtlas(
            renderData.textures,
            textureAtlasFileName(
                renderData.theme
            )
        );

        //* Register textures
        TexturesModule::registerTexture( renderData.textures, RenderId::NONE, { 0, 0 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::CURSOR, { 35, 0 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::HERO, { 70, 0 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::WALL, { 105, 0 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::FLOOR, { 0, 35 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::DESCEND, { 35, 35 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::ASCEND, { 70, 35 } );
        TexturesModule::registerTexture( renderData.textures, RenderId::GOBLIN, { 105, 35 } );
    }

    void renderTile(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Color const& tint
    )
    {
        //* Use 0.5f pixel offset to avoid texture bleeding
        DrawTexturePro(
            textures.atlas,
            Rectangle{
                TexturesModule::getTexturePosition(
                    textures,
                    renderId
                )
                        .x
                    + 0.5f,
                TexturesModule::getTexturePosition(
                    textures,
                    renderId
                )
                        .y
                    + 0.5f,
                TextureData::textureSize - ( 2 * 0.5f ),
                TextureData::textureSize - ( 2 * 0.5f )
            },
            Rectangle{
                worldPixel.x,
                worldPixel.y,
                TileData::tileSize,
                TileData::tileSize
            },
            //* TileData::TILE_CENTER,
            Vector2{ 0, 0 },
            0,
            tint
        );
    }

    void renderToChunk(
        Textures const& textures,
        RenderId renderId,
        Vector2 const& worldPixel,
        Chunk& chunk,
        Color const& tint
    )
    {
        //* Use 0.5f pixel offset to avoid texture bleeding
        DrawTexturePro(
            textures.atlas,
            Rectangle{
                TexturesModule::getTexturePosition(
                    textures,
                    renderId
                )
                        .x
                    + 0.5f,
                TexturesModule::getTexturePosition(
                    textures,
                    renderId
                )
                        .y
                    + 0.5f,
                TextureData::textureSize - ( 2 * 0.5f ),
                TextureData::textureSize - ( 2 * 0.5f )
            },
            Rectangle{
                worldPixel.x - chunk.position.x,
                worldPixel.y - chunk.position.y,
                TileData::tileSize,
                TileData::tileSize
            },
            //* TileData::TILE_CENTER,
            Vector2{ 0, 0 },
            0,
            tint
        );
    }

    void renderChunk( Chunk const& chunk )
    {
        DrawTexturePro(
            chunk.renderTexture.texture,
            Rectangle{
                0,
                0,
                static_cast<float>( chunk.renderTexture.texture.width ),
                static_cast<float>( -chunk.renderTexture.texture.height )
            },
            Rectangle{
                chunk.position.x,
                chunk.position.y,
                ChunkData::chunkSize_f,
                ChunkData::chunkSize_f
            },
            Vector2{ 0, 0 },
            0,
            WHITE
        );

#if defined( DEBUG ) && defined( DEBUG_CHUNKS )
        DrawRectangleLinesEx(
            Rectangle{
                chunk.position.x,
                chunk.position.y,
                ChunkData::CHUNK_SIZE_F,
                ChunkData::CHUNK_SIZE_F
            },
            3.0f,
            YELLOW
        );
#endif
    }

    void renderFog(
        Vector2 const& fogPosition,
        Fog fog
    )
    {
        Color tint{};

#if defined( DEBUG ) && defined( DEBUG_VISIBILITY )
        if ( fog == Fog::OPAQUE )
        {
            tint = ColorAlpha( RED, 0.5f );
        }

        else
        {
            tint = ColorAlpha( BLUE, 0.5f );
        }
#else
        if ( fog == Fog::OPAQUE )
        {
            tint = BLACK;
        }

        else
        {
            tint = ColorAlpha( BLACK, 0.5f );
        }
#endif

        DrawRectangleV(
            fogPosition,
            TileData::tileDimensions,
            tint
        );
    }

    void cycleThemes( size_t& theme )
    {
        theme = ( theme < TextureData::themes.size() - 1 ) ? ++theme : 0;
    }

    void deinit( Textures& textures )
    {
        TexturesModule::unloadAtlas( textures );
    }
}
