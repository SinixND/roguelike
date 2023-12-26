#ifndef RENDER_H_20231217205005
#define RENDER_H_20231217205005

#include "Component.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "System.h"
#include <iostream>
#include <raylib.h>

namespace snd
{
    class Render : public System<Render>
    {
    public:
        void action() { std::cout << "DUMMY: Execute render action...\n"; };

        Render()
        {
            signature_.set(Texture::getId());
            signature_.set(Position::getId());
        };
    };
}

#endif