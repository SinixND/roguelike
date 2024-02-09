#ifndef CURSOR_H_20240208011600
#define CURSOR_H_20240208011600

#include "Attributes/Position.h"
#include "Attributes/RenderId.h"

class Cursor
{
public:
    RenderId renderId{CURSOR};
    Position position{};
};

#endif