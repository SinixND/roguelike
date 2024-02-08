#include "Cursor.h"

#include "Components/Position.h"
#include "Components/RenderId.h"
#include "IEntity.h"
#include "RuntimeDatabase.h"

// public:

// public:
Position* Cursor::getPosition() { return position_; }
void Cursor::setPosition(Position position) { position_ = ecs_.assignComponent<Position>(id_, position); }

void Cursor::updatePosition()
{
    position_ = Vector2Add(convertToTile(GetMousePosition()), transform);
}

Cursor::Cursor(snd::ECS& ecs)
    : IEntity(ecs)
{
}

// private:
