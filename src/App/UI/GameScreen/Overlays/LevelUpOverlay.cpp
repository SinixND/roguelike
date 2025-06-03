#include "LevelUpOverlay.h"

#include "AttributeSystem.h"
#include "AttributesComponent.h"
#include "ColorData.h"
#include "Game.h"
#include "GameFont.h"
#include "Hero.h"
#include "raylibEx.h"
#include <raylib.h>
#include <string>

void LevelUpOverlay::init()
{
    int renderWidth{ GetRenderWidth() };
    int renderHeight{ GetRenderHeight() };

    panel.setOuter(
        RectangleEx{
            0.25f * renderWidth,
            0.25f * renderHeight,
            0.5f * static_cast<float>( renderWidth ),
            0.5f * static_cast<float>( renderHeight )
        },
        0,
        0.5f * GameFont::fontSize
    );
}

void LevelUpOverlay::update( Game const& game )
{
    DrawRectangleRec(
        panel.box().rectangle(),
        ColorData::BG
    );

    DrawTextExCentered(
        GameFont::font(),
        "Level Up!",
        panel.inner(),
        -4 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    DrawTextExCentered(
        GameFont::font(),
        "1 proficiency point to spend.",
        panel.inner(),
        -2 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    DrawTextExCentered(
        GameFont::font(),
        "Choose or select attribute:",
        panel.inner(),
        -1 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    int selection{};

    //* Attribute options
    selection = ( game.selectedAttribute == 1 );

    std::string opt1{ ( selection ) ? "[x]" : "[ ]" };
    opt1 += TextFormat(
        " VIT [%][i%]",
        game.hero.attributes.vitality + selection,
        100 * ( game.hero.attributes.vitality + selection ) / ( AttributesModule::totalPoints( game.hero.attributes ) + 1 )
    );

    DrawTextExCentered(
        GameFont::font(),
        opt1.c_str(),
        panel.inner(),
        1 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    selection = ( game.selectedAttribute == 2 );

    std::string opt2{ ( selection ) ? "[x]" : "[ ]" };
    opt2 += TextFormat(
        " STR [%][i%]",
        game.hero.attributes.strength + selection,
        100 * ( game.hero.attributes.strength + selection ) / ( AttributesModule::totalPoints( game.hero.attributes ) + 1 )
    );

    DrawTextExCentered(
        GameFont::font(),
        opt2.c_str(),
        panel.inner(),
        2 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    selection = ( game.selectedAttribute == 3 );

    std::string opt3{ ( selection ) ? "[x]" : "[ ]" };
    opt3 += TextFormat(
        " DEF [%][i%]",
        game.hero.attributes.defense + selection,
        100 * ( game.hero.attributes.defense + selection ) / ( AttributesModule::totalPoints( game.hero.attributes ) + 1 )
    );

    DrawTextExCentered(
        GameFont::font(),
        opt3.c_str(),
        panel.inner(),
        3 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    selection = ( game.selectedAttribute == 4 );

    std::string opt4{ ( selection ) ? "[x]" : "[ ]" };
    opt4 += TextFormat(
        " AGI [%][i%]",
        game.hero.attributes.agility + ( selection ),
        100 * ( game.hero.attributes.agility + ( selection ) ) / ( AttributesModule::totalPoints( game.hero.attributes ) + 1 )
    );

    DrawTextExCentered(
        GameFont::font(),
        opt4.c_str(),
        panel.inner(),
        4 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    DrawRectangleLinesEx(
        panel.outer().rectangle(),
        1,
        ColorData::BORDER
    );
}
