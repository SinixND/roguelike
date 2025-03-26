#ifndef IG20241112230213
#define IG20241112230213

#include "Textures.h"
#include <cstddef>

struct RenderData
{
    Textures textures{};

    //* Themes: TextureData::themes
    size_t theme{ 1 };
};

#endif
