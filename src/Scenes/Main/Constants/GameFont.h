#ifndef IG20240601015436
#define IG20240601015436

#include <raylib.h>

class GameFont
{
    static inline Font font_{};

public:
    static inline float const FONT_HEIGHT{24};
    //* raygui font padding is 4
    static inline float fontWidth{};

public:
    static void load()
    {
        font_ = LoadFontEx(
            "resources/fonts/LiberationMono-Regular.ttf",
            GameFont::FONT_HEIGHT,
            nullptr,
            0);

        fontWidth = static_cast<float>(
            MeasureTextEx(
                font_,
                "1",
                FONT_HEIGHT,
                0)
                .x);
    }

    static void unload() { UnloadFont(font_); }

public:
    static Font const& font()
    {
        return font_;
    }
};

#endif
