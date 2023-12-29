#ifndef TEXTURE_H_20231220001155
#define TEXTURE_H_20231220001155

#include "Component.h"
#include <raylib.h>
#include <string>

namespace snd
{
    struct TextureComponent : Component<TextureComponent>
    {
        Texture2D* texture_;

        TextureComponent(Texture2D* texture)
            : texture_{texture} {};
    };
}

#endif