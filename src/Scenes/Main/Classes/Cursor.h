#ifndef IG20240531185351
#define IG20240531185351

#include "PositionComponent.h"
#include "RenderId.h"

struct Camera2D;
typedef struct Camera2D Camera2D;
struct Vector2I;

class Cursor
{
public:
    PositionComponent position{};
    RenderId renderId{RenderId::CURSOR};
    bool isActive{false};

public:
    void toggle();

    void update(
        Camera2D const& camera,
        Vector2I const& heroPosition);
};

#endif
