#ifndef RENDER_H_20231217205005
#define RENDER_H_20231217205005

#include "Component.h"
#include "ComponentManager.h"
#include "Id.h"
#include "System.h"
#include "Texture.h"
#include <raylib.h>

namespace snd
{
    class Render : public System
    {
        Render()
            : System{Component<Texture>::getId()} {};

        void action(EntityId entity) override{};
    };
}

#endif