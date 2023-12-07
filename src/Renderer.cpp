#include "Renderer.h"

#include "Renderable.h"

namespace snd
{
    Renderer::~Renderer(){};

    void Renderer::render(Renderable& renderable)
    {
        renderable.render();
    }
}