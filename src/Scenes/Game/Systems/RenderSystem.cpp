#include "RenderSystem.h"
#include "PanelSystem.h"

//* #define DEBUG_CHUNKS
#define DEBUG_VISIBILITY

#include "Chunk.h"
#include "ChunkData.h"
#include "Fog.h"
#include "GameFont.h"
#include "GameScenePanels.h"
#include "Hero.h"
#include "Logger.h"
#include "Objects.h"
#include "RenderData.h"
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
    RenderData const& loadRenderData( RenderData& renderData )
    {
        //* Load texture atlas
        renderData.textures = TexturesModule::loadAtlas(
            renderData.textures,
            textureAtlasFileName(
                renderData.theme
            )
        );

        //* Register textures
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::NONE, { 0, 0 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::CURSOR, { 35, 0 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::HERO, { 70, 0 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::WALL, { 105, 0 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::FLOOR, { 0, 35 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::DESCEND, { 35, 35 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::ASCEND, { 70, 35 } );
        renderData.textures = TexturesModule::registerTexture( renderData.textures, RenderId::GOBLIN, { 105, 35 } );

        return renderData;
    }

    void renderTexture(
        Textures const& textures,
        Vector2 const& worldPixel,
        RenderId renderId,
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

    void renderStatusPanel(
        Panel const& statusPanel,
        Hero const& hero,
        int mapLevel,
        Color const& bgColor,
        float borderWidth
    )
    {
        PanelSystem::drawPanelBackground( statusPanel, bgColor );
        PanelSystem::drawPanelBorder( statusPanel, borderWidth );

        DrawTextEx(
            GameFont::font(),
            TextFormat(
                "Maplevel:%2i   Lvl:%3i   EXP:%5i/%5i   HP:%3i/%3i   ATK:%3i",
                mapLevel,
                hero.experience.expLevel,
                hero.experience.expCurrent,
                hero.experience.levelUpThreshold,
                hero.health.currentHealth,
                hero.health.maxHealth,
                DamageModule::damageAverage( hero.damage )
            ),
            Vector2{
                statusPanel.inner().left(),
                statusPanel.inner().top()
            },
            GameFont::fontSize,
            GuiGetStyle( DEFAULT, TEXT_SPACING ),
            RAYWHITE
        );
    }

    void renderInfoPanel(
        Panel const& infoPanel,
        Objects const& objects,
        Vector2I const& cursorPosition,
        Color const& bgColor,
        float borderWidth
    )
    {
        PanelSystem::drawPanelBackground( infoPanel, bgColor );

        if ( !objects.ids.contains( cursorPosition ) )
        {
            return;
        }

        PanelSystem::drawPanelBorder( infoPanel, borderWidth );

        size_t objectIdx{ objects.ids.index( cursorPosition ) };

        //* Draw tag and action from tile under cursor
        DrawTextEx(
            GameFont::font(),
            TextFormat( "[Object] %s: %s", objects.names[objectIdx].c_str(), objects.actions[objectIdx].c_str() ),
            Vector2{
                infoPanel.inner().left(),
                infoPanel.inner().top()
            },
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );
    }

    void renderLogPanel(
        Panel const& logPanel,
        Color const& bgColor,
        float borderWidth
    )
    {
        if ( !snx::Logger::hasTurnMessage() )
        {
            return;
        }

        PanelSystem::drawPanelBackground( logPanel, bgColor );
        PanelSystem::drawPanelBorder( logPanel, borderWidth );

        float lines{ ( logPanel.inner().height() / ( 1.5f * GameFont::fontSize ) ) - 1 };

        for ( int n{ 0 }; n < lines; ++n )
        {
            DrawTextEx(
                GameFont::font(),
                snx::Logger::getMessage( n ).c_str(),
                Vector2{
                    logPanel.inner().left(),
                    logPanel.inner().top() + ( n * 1.5f * GameFont::fontSize )
                },
                GameFont::fontSize,
                0,
                LIGHTGRAY
            );
        }
    }

    size_t cycleThemes( size_t theme )
    {
        theme = ( theme < TextureData::themes.size() - 1 ) ? ++theme : 0;

        return theme;
    }

    void deinit( Textures const& textures )
    {
        TexturesModule::unloadAtlas( textures );
    }
}
