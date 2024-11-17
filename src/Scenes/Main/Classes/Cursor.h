#ifndef IG20240531185351
#define IG20240531185351

#include "RenderId.h"
#include <raylib.h>

struct Camera2D;
typedef struct Camera2D Camera2D;
struct Vector2I;

class Cursor
{
public:
    Vector2 position{};
    RenderId renderId{RenderId::CURSOR};
    bool isActive{false};

public:
    void toggle();

    void update(
        Camera2D const& camera,
        Vector2I const& heroPosition);
};

#endif
