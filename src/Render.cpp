#include "Render.h"

#include "Renderable.h"

namespace sxd
{
    void Render::render(Renderable& renderable)
    {
        renderable.render();
    }
}