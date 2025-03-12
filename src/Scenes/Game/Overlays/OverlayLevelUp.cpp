#include "OverlayLevelUp.h"

#include "AttributeSystem.h"
#include "AttributesComponent.h"
#include "Colors.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "ExperienceSystem.h"
#include "GameFont.h"
#include "Hero.h"
#include "InputId.h"
#include "raylibEx.h"
#include <raylib.h>

char options[ATTRIBUTES + 1]{
    '0',
    'V',
    'A'
};

[[nodiscard]]
Hero const& levelUpHero(
    Hero& hero,
    char selection
)
{
    switch ( selection )
    {
        default:
            return hero;

        //* Vitality
        case 'V':
        case 'v':
        {
            ++hero.attributes.vitality;

            break;
        }

        //* Agility
        case 'A':
        case 'a':
        {
            ++hero.attributes.agility;

            break;
        }
    }

    // ExperienceSystem::updateStats( hero.health );

    AttributeSystem::udpateStats(
        hero.health,
        hero.energy,
        hero.attributes
    );

    hero.health.current = hero.health.maximum;

    snx::EventDispatcher::notify( EventId::LEVELED_UP );

    return hero;
}

namespace OverlayLevelUpModule
{
    OverlayLevelUp const& init( OverlayLevelUp& overlay )
    {
        int renderWidth{ GetRenderWidth() };
        int renderHeight{ GetRenderHeight() };

        overlay.panel.setOuter(
            RectangleEx{
                0.25f * renderWidth,
                0.25f * renderHeight,
                0.5f * static_cast<float>( renderWidth ),
                0.5f * static_cast<float>( renderHeight )
            },
            0,
            0.5f * GameFont::fontSize
        );

        return overlay;
    }

    OverlayLevelUp const& update(
        OverlayLevelUp& overlay,
        Hero& heroIO,
        InputId currentInputId
    )
    {
        char selection = GetCharPressed();
        // int selection{ 0 };

        switch ( currentInputId )
        {
            default:
                break;

            case InputId::ACT_DOWN:
            {
                ++overlay.selectedOption;

                break;
            }

            case InputId::ACT_UP:
            {
                --overlay.selectedOption;

                break;
            }

            case InputId::ACT_IN_PLACE:
            {
                if ( !selection )
                {
                    selection = options[overlay.selectedOption];
                }

                break;
            }
        }

        if ( overlay.selectedOption > ATTRIBUTES )
        {
            overlay.selectedOption = 1;
        }
        else if ( overlay.selectedOption < 1 )
        {
            overlay.selectedOption = ATTRIBUTES;
        }

        DrawRectangleRec(
            overlay.panel.box().rectangle(),
            Colors::BG
        );

        DrawTextExCentered(
            GameFont::font(),
            "LevelUp!",
            overlay.panel.inner(),
            -4 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            "1 Attribute point to spend.",
            overlay.panel.inner(),
            -2 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            "Choose or select attribute:",
            overlay.panel.inner(),
            -1 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        //* Options
        DrawTextExCentered(
            GameFont::font(),
            ( overlay.selectedOption == 1 ) ? "> [V]itality" : "[V]itality",
            overlay.panel.inner(),
            1 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            ( overlay.selectedOption == 2 ) ? "> [A]gility" : "[A]gility",
            overlay.panel.inner(),
            2 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawRectangleLinesEx(
            overlay.panel.outer().rectangle(),
            1,
            Colors::BORDER
        );

        heroIO = levelUpHero( heroIO, selection );

        return overlay;
    }
}
