#ifndef RANGE_H_20231217185913
#define RANGE_H_20231217185913

#include "Component.h"
namespace snd
{
    struct Range : Component<Range>
    {
        float range_;

        Range(float rangeValue)
            : range_{rangeValue} {};
    };
}

#endif