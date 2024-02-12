#ifndef _20240211172120
#define _20240211172120

#include "RenderData.h"
#include "raylibEx.h"

class Entity
{
public:
    Vector2Int position_{};
    RenderData renderData{};
};

#endif