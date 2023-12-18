#ifndef DIRECTION_H_20231218005758
#define DIRECTION_H_20231218005758

#include "Component.h"
#include <raylib.h>

namespace snd
{
    struct Orientation : Component<Orientation>
    {
        Vector2 orientation_;

        Orientation(Vector2 orientation)
            : orientation_{orientation} {};
    };
}

#endif