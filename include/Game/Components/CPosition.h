#ifndef CPOSITION_H_20240128153104
#define CPOSITION_H_20240128153104

#include "Component.h"
#include <raylibEx.h>

struct CPosition
    : public snd::Component<CPosition>
{
public:
    Vector2Int& getPosition() { return position_; };
    void setPosition(const Vector2Int& position)
    {
        position_ = position;
    };

    CPosition(float x, float y)
        : position_(static_cast<int>(x), static_cast<int>(y)){};

    CPosition(int x, int y)
        : position_(x, y){};

    CPosition(Vector2Int position = {0, 0})
        : position_(position){};

private:
    Vector2Int position_;
};

#endif