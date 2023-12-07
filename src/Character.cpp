#include "Character.h"

#include <raylib.h>

namespace snd
{

    Character::Character(Vector2 position, float size, Color color)
        : position_{position}
        , size_{size}
        , color_{color}
    {
    }

    Vector2& Character::getPosition()
    {
        return position_;
    }

    void Character::setPosition(Vector2 position)
    {
        position_ = position;
    }

    void Character::move(Vector2 direction, float distance)
    {
        position_.x += direction.x * distance;
        position_.y += direction.y * distance;
    }

    void Character::render()
    {
        DrawCircleV(position_, size_, color_);
    }
}