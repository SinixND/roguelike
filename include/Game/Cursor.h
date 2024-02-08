#ifndef CURSOR_H_20240208011600
#define CURSOR_H_20240208011600

#include "IEntity.h"
#include "RuntimeDatabase.h"

class RenderId;
class Position;

class Cursor : public IEntity
{
public:
    RenderId* renderId{ecs_.assignComponent<RenderId>(id_, CURSOR)};

public:
    Position* getPosition();
    void setPosition(Position position);

    void updatePosition();

    Cursor(snd::ECS& ecs);

private:
    Position* position_{};
};

#endif