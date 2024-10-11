#ifndef IG20240601015436
#define IG20240601015436

#include <raylib.h>

class GameFont
{
    static inline Font font_{};

public:
    static inline float const fontHeight{16};
    // raygui font padding is 4
    static inline float const fontWidth{fontHeight - 8};

public:
    static void load()
    {
        font_ = LoadFontEx(
            "resources/fonts/LiberationMono-Regular.ttf",
            GameFont::fontHeight,
            nullptr,
            0);
    }

    static void unload() { UnloadFont(font_); }

    static Font const& font()
    {
        return font_;
    }
};

#endif
