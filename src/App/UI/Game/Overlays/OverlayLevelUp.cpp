#include "OverlayLevelUp.h"

#include "AttributeSystem.h"
#include "AttributesComponent.h"
#include "DataColors.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "ExperienceSystem.h"
#include "GameFont.h"
#include "Hero.h"
#include "InputId.h"
#include "raylibEx.h"
#include <cstring>
#include <raylib.h>
#include <string>

char constexpr ATTRIBUTE_CHOICES[ATTRIBUTES + 1]{
    '0',
    'v',
    's',
    'd',
    'a',
};

[[nodiscard]]
Hero const& levelUpHero(
    Hero& hero,
    char attributeChoice
)
{
    switch ( attributeChoice )
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

    ExperienceSystem::updateStats( hero.health );

    AttributeSystem::updateStats(
        hero.health,
        hero.energy,
        hero.attributes
    );

    hero.health.current = hero.health.maximum;

    if ( ( AttributesModule::totalPoints( hero.attributes ) - ( ATTRIBUTES * BASE_POINTS ) ) < hero.experience.level )
    {
        return hero;
    }
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
        char attributeChoice{ 0 };
        // int selection{ 0 };

        switch ( currentInputId )
        {
            default:
                // selection = GetCharPressed();
                break;

            case InputId::ACT_DOWN:
            {
                ++overlay.selectedAttribute;

                break;
            }

            case InputId::ACT_UP:
            {
                --overlay.selectedAttribute;

                break;
            }

            case InputId::ACT_IN_PLACE:
            {
                if ( !attributeChoice )
                {
                    attributeChoice = ATTRIBUTE_CHOICES[overlay.selectedAttribute];
                }

                break;
            }
        }

        if ( overlay.selectedAttribute > ATTRIBUTES )
        {
            overlay.selectedAttribute = 1;
        }
        else if ( overlay.selectedAttribute < 1 )
        {
            overlay.selectedAttribute = ATTRIBUTES;
        }

        DrawRectangleRec(
            overlay.panel.box().rectangle(),
            Data::Colors::BG
        );

        DrawTextExCentered(
            GameFont::font(),
            "Level Up!",
            overlay.panel.inner(),
            -4 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            "1 proficiency point to spend.",
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
        std::string opt1{ ( overlay.selectedAttribute == 1 ) ? "[x]" : "[ ]" };
        opt1 += TextFormat(
            "[%i%] VIT",
            100 * ( heroIO.attributes.vitality + ( overlay.selectedAttribute == 1 ) ) / AttributesModule::totalPoints( heroIO.attributes )
        );

        DrawTextExCentered(
            GameFont::font(),
            opt1.c_str(),
            overlay.panel.inner(),
            1 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        std::string opt2{ ( overlay.selectedAttribute == 2 ) ? "[x]" : "[ ]" };
        opt2 += TextFormat(
            "[%i%] AGI",
            100 * ( heroIO.attributes.agility + ( overlay.selectedAttribute == 2 ) ) / AttributesModule::totalPoints( heroIO.attributes )
        );
        DrawTextExCentered(
            GameFont::font(),
            opt2.c_str(),
            overlay.panel.inner(),
            2 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawRectangleLinesEx(
            overlay.panel.outer().rectangle(),
            1,
            Data::Colors::BORDER
        );

        heroIO = levelUpHero( heroIO, attributeChoice );

        return overlay;
    }
}
