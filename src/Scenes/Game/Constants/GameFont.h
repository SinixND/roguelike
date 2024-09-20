#ifndef IG20240601015436
#define IG20240601015436

#include <raylib.h>

class GameFont
{
    static inline Font font_{};

public:
    static inline float const fontSize{16};

public:
    static void load() { font_ = LoadFont("resources/fonts/LiberationMono-Regular.ttf"); }

    static void unload() { UnloadFont(font_); }

    static Font const& font()
    {
        return font_;
    }
};

#endif
