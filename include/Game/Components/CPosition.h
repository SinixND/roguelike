#ifndef CPOSITION_H_20240128153104
#define CPOSITION_H_20240128153104

#include "Component.h"
#include <raylib.h>

struct CPosition
    : public snd::Component<CPosition>
{
public:
    Vector2& getPosition() { return position_; };
    void setPosition(const Vector2& position) { position_ = position; };

    CPosition(float x, float y)
        : position_(x, y){};

    CPosition(int x, int y)
        : position_(static_cast<float>(x), static_cast<float>(y)){};

    CPosition(Vector2 position = {0, 0})
        : position_(position){};

private:
    Vector2 position_;
};

#endif