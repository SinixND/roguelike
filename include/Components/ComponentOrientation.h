#ifndef COMPONENTORIENTATION_H_20240105191159
#define COMPONENTORIENTATION_H_20240105191159

#include "Component.h"
#include <raylib.h>

namespace snd
{
    typedef enum
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
    } Direction;

    struct DirectionComponent : public Component<DirectionComponent>
    {
        Vector2 directionVector_;
        float directionDeg_;

        DirectionComponent(Direction direction)
        {
            switch (direction)
            {
            default:
            case RIGHT:
                directionVector_ = {1, 0};
                directionDeg_ = 0;
                break;

            case LEFT:
                directionVector_ = {-1, 0};
                directionDeg_ = 180;
                break;

            case UP:
                directionVector_ = {0, -1};
                directionDeg_ = -90;
                break;

            case DOWN:
                directionVector_ = {0, 1};
                directionDeg_ = 90;
                break;
            }
        }
    };
}

#endif