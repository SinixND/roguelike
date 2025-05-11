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

    //* Options
    std::string opt1{ ( game.selectedAttribute == 1 ) ? "[x]" : "[ ]" };
    opt1 += TextFormat(
        "[%i%] VIT",
        100 * ( game.hero.attributes.vitality + ( game.selectedAttribute == 1 ) ) / AttributesModule::totalPoints( game.hero.attributes )
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

    std::string opt2{ ( game.selectedAttribute == 2 ) ? "[x]" : "[ ]" };
    opt2 += TextFormat(
        "[%i%] AGI",
        100 * ( game.hero.attributes.agility + ( game.selectedAttribute == 2 ) ) / AttributesModule::totalPoints( game.hero.attributes )
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

    DrawRectangleLinesEx(
        panel.outer().rectangle(),
        1,
        ColorData::BORDER
    );
}
