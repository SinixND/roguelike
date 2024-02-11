#ifndef _20240211172120
#define _20240211172120

#include "Attributes/RenderId.h"
#include "raylib.h"
#include "raylibEx.h"

struct Entity
{
    Vector2Int position_{};
    RenderId renderId_{};
};

#endif