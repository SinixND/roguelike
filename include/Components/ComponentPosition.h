#ifndef POSITION_H_20231215002439
#define POSITION_H_20231215002439

#include "Component.h"
#include "raylib.h"

namespace snd
{
    struct PositionComponent : public Component<PositionComponent>
    {
        Vector2 position_;

        PositionComponent(float x, float y)
            : position_{x, y} {};

        PositionComponent(Vector2 position = {0, 0})
            : position_{position} {};
    };
}

#endif