#ifndef IG20240601015436
#define IG20240601015436

#include "raygui.h"
#include <raylib.h>

class GameFont
{
    static inline Font font_{};

public:
    static inline float fontSize{ 16 };
    static inline float fontWidth{};

public:
    static void load()
    {
        font_ = LoadFontEx(
            "assets/fonts/LiberationMono-Regular.ttf",
            GameFont::fontSize,
            nullptr,
            0
        );

        fontWidth = static_cast<float>(
            MeasureTextEx(
                font_,
                "X",
                fontSize,
                GuiGetStyle( DEFAULT, TEXT_SPACING )
            )
                .x
        );
    }

    static void unload() { UnloadFont( font_ ); }

    static Font const& font()
    {
        return font_;
    }
};

#endif
