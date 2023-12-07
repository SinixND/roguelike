#ifndef RENDERER_H_20231203231241
#define RENDERER_H_20231203231241

namespace snd
{
    class Renderable;

    class Renderer
    {
    public:
        virtual ~Renderer();
        void render(Renderable& renderable);
    };
}

#endif