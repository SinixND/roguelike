#ifndef TEXTURE_H_20231220001155
#define TEXTURE_H_20231220001155

#include "Component.h"
#include <raylib.h>
#include <string>

namespace snd
{
    struct Texture : Component<Texture>
    {
        Texture2D* texture_;

        Texture(Texture2D* texture)
            : texture_{texture} {};
    };
}

#endif