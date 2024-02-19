#ifndef _20240211172120
#define _20240211172120

#include "Graphic.h"
#include "raylibEx.h"

class GameObject
{
public:
    Vector2Int& position() { return position_; };
    void setPosition(const Vector2Int& position)
    {
        position_ = position;
    };

    Graphic& graphic() { return graphic_; };

private:
    Vector2Int position_{};
    Graphic graphic_{};
};

#endif