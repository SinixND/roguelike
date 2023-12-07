#include "Render.h"

#include "Renderable.h"

namespace snd
{
    void Render::render(Renderable& renderable)
    {
        renderable.render();
    }
}