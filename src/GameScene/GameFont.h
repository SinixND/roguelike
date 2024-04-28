#ifndef IG20240426003510
#define IG20240426003510

#include <raylib.h>

class GameFont
{
private:
    Font font_{};

public:
    Font const& operator()() { return font_; }
    void loadFont(char const* fileName) { font_ = LoadFont(fileName); }
    void unloadFont() { UnloadFont(font_); }
};

#endif