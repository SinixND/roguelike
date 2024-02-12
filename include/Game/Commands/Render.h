#ifndef _20240212001834
#define _20240212001834

#include "ICommand.h"
#include "RenderId.h"
#include "Renderer.h"

class Render : public ICommand
{
public:
    void execute() const override { receiver_.render(renderId_, x_, y_); };

    Render(Renderer& receiver, RenderId renderId, int x, int y)
        : receiver_(receiver)
        , renderId_(renderId)
        , x_(x)
        , y_(y)
    {
    }

private:
    Renderer& receiver_;

    RenderId renderId_{};
    int x_;
    int y_;
};

#endif