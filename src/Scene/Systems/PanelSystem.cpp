#include "PanelSystem.h"

#include "EventId.h"
#include "GameFont.h"
#include "Hero.h"
#include "Logger.h"
#include "Objects.h"
#include "Panels.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <string>

int constexpr INFO_PANEL_CHAR_WIDTH{ 16 };

std::string formatInfo( std::string const& text )
{
    std::string info{ "|" };

    info.append( text );

    while ( info.length() < ( INFO_PANEL_CHAR_WIDTH - 2 ) )
    {
        info.append( " " );
    }

    info.append( "|\n" );

    return info;
}

namespace PanelSystem
{
    [[nodiscard]]
    Panels const& init( Panels& panels )
    {
        std::string widthString{};

        for ( int n{ 0 }; n < INFO_PANEL_CHAR_WIDTH; ++n )
        {
            widthString.append( "I" );
        }

        float heroInfoPanelWidth =
            MeasureTextEx(
                GameFont::font(),
                widthString.c_str(),
                GameFont::fontHeight,
                0
            )
                .x;

        float gameInfoPanelHeight =
            static_cast<float>(
                ( /*Number of lines to print*/ 1 * GameFont::fontHeight )
                + GameFont::fontHeight
            );

        float logPanelHeight =
            static_cast<float>(
                ( /*Number of lines to print*/ 4 * GameFont::fontHeight )
                + GameFont::fontHeight
            );

        panels.tileInfo
            .setRight( static_cast<int>( GetRenderWidth() ) )
            .setBottom( static_cast<int>( GetRenderHeight() ) )
            .resizeWidthLeft( heroInfoPanelWidth )
            .resizeHeightTop( logPanelHeight );

        panels.heroInfo
            .setRight( static_cast<int>( GetRenderWidth() ) )
            .setBottom( panels.tileInfo.top() )
            .resizeWidthLeft( heroInfoPanelWidth );

        panels.status
            .setRight( panels.tileInfo.left() )
            .setBottom( gameInfoPanelHeight );

        panels.log
            .setRight( panels.tileInfo.left() )
            .setBottom( static_cast<int>( GetRenderHeight() ) )
            .resizeHeightTop( logPanelHeight );

        panels.map
            .setRight( panels.tileInfo.left() )
            .setBottom( panels.log.top() )
            .setTop( panels.status.bottom() );

        snx::PublisherStatic::publish( EventId::PANELS_RESIZED );

        return panels;
    }

    void drawGameInfoPanelContent(
        Panels const& panels,
        int level
    )
    {
        //* Draw text for current level
        char const* currentLevel{ TextFormat( "Level %i", level ) };

        Font const& font{ GameFont::font() };

        float fontSize{ GameFont::fontHeight };

        Vector2 textDimensions{
            MeasureTextEx(
                font,
                currentLevel,
                fontSize,
                0
            )
        };

        DrawTextEx(
            font,
            currentLevel,
            Vector2{
                ( ( 0.5f * panels.map.width() ) - ( 0.5f * textDimensions.x ) ),
                ( 0.5f * fontSize )
            },
            fontSize,
            GuiGetStyle( DEFAULT, TEXT_SPACING ),
            RAYWHITE
        );
    }

    void drawHeroInfoPanelContent(
        Panels const& panels,
        [[maybe_unused]] Hero const& hero
    )
    {
        int fontSize{ GuiGetStyle( DEFAULT, TEXT_SIZE ) };
        /*
        |= HERO ======|
        |HP : 100/100 |
        |ATK: 2       |
        |             |
        |> INVENTORY  |
        |HEA: RUSTY   |
        |_____________|
        */
        std::string info{};

        info.append( "|= HERO ======|\n" );

        info.append(
            formatInfo(
                "HP : "
                + std::to_string( hero.health.currentHealth )
                + "/"
                + std::to_string( hero.health.maxHealth ).c_str()
            )
        );

        info.append( formatInfo(
            "ATK : "
            + std::to_string( hero.damage.baseDamage )
        ) );

        info.append( "|_____________|" );

        DrawTextEx(
            GameFont::font(),
            TextFormat(
                info.c_str()
            ),
            Vector2{
                panels.heroInfo.left() + ( 0.5f * GameFont::fontWidth ),
                panels.heroInfo.top() + ( 0.5f * GameFont::fontHeight )
            },
            fontSize,
            0,
            LIGHTGRAY
        );
    }

    void drawTileInfoPanelContent(
        Panels const& panels,
        Objects const& objects,
        Vector2I const& cursorPosition
    )
    {
        if ( !objects.ids.contains( cursorPosition ) )
        {
            return;
        }

        size_t objectId{ objects.ids.at( cursorPosition ) };

        int fontSize{ GuiGetStyle( DEFAULT, TEXT_SIZE ) };

        //* Draw tag and action from tile under cursor
        DrawTextEx(
            GameFont::font(),
            TextFormat( "Object: %s\nAction: %s", objects.names.at( objectId ).c_str(), objects.actions.at( objectId ).c_str() ),
            Vector2{
                panels.tileInfo.left() + ( 0.5f * GameFont::fontWidth ),
                panels.tileInfo.top() + ( 0.5f * fontSize )
            },
            fontSize,
            0,
            LIGHTGRAY
        );
    }

    void drawLogPanelContent( Panels const& panels )
    {
        int fontSize{ GuiGetStyle( DEFAULT, TEXT_SIZE ) };

        double lines{ ( panels.log.height() / ( 1.5 * fontSize ) ) - 1 };

        for ( int n{ 0 }; n < lines; ++n )
        {
            DrawTextEx(
                GameFont::font(),
                snx::Logger::getMessage( n ).c_str(),
                Vector2{
                    panels.log.left() + ( 0.5f * fontSize ),
                    panels.log.bottom() - ( fontSize * 1.5f ) - ( n * 1.5f * fontSize )
                },
                fontSize,
                0,
                LIGHTGRAY
            );
        }
    }

    void drawPanelBorders( Panels const& panels )
    {
        float panelBorderWeight = 1;

        DrawRectangleLinesEx(
            panels.tileInfo.rectangle(),
            panelBorderWeight,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.heroInfo.rectangle(),
            panelBorderWeight,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.status.rectangle(),
            panelBorderWeight,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.log.rectangle(),
            panelBorderWeight,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.map.rectangle(),
            panelBorderWeight,
            Color{ 25, 25, 25, 255 }
        );
    }
}
