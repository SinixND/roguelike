#ifndef POSITION_H_20231215002439
#define POSITION_H_20231215002439

#include "Component.h"

namespace snd
{
    struct PositionComponent : public Component<PositionComponent>
    {
        PositionComponent(float x, float y)
            : x_{x}
            , y_{y} {};

        float x_;
        float y_;
    };
}

#endif