#ifndef IG20240601015436
#define IG20240601015436

#include <raylib.h>

class GameFont
{
    static inline Font font_{};

public:
    static inline float const fontHeight{ 24 };
    //* raygui font padding is 4
    static inline float fontWidth{};

public:
    static void load()
    {
        font_ = LoadFontEx(
            "assets/fonts/LiberationMono-Regular.ttf",
            GameFont::fontHeight,
            nullptr,
            0
        );

        fontWidth = static_cast<float>(
            MeasureTextEx(
                font_,
                "1",
                fontHeight,
                0
            )
                .x
        );
    }

    static void unload() { UnloadFont( font_ ); }

public:
    static Font const& font()
    {
        return font_;
    }
};

#endif
