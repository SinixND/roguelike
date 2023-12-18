#ifndef SPRITE_H_20231217205122
#define SPRITE_H_20231217205122

#include "Component.h"
#include <raylib.h>
#include <string>

namespace snd
{
    struct Texture : Component<Texture>
    {
        Texture2D texture_;

        Texture(Texture2D texture)
            : texture_{texture} {};
    };
}

#endif