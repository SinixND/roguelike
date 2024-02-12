#ifndef _20240212001513
#define _20240212001513

#include "LayerId.h"
#include "RenderId.h"

struct RenderData
{
    RenderId renderId_{};
    LayerId layerId_{};
};

#endif