#include "Renderer.h"

#include "Renderable.h"

namespace snd
{
    void Renderer::execute(Renderable& renderable)
    {
        renderable.render();
    }
}