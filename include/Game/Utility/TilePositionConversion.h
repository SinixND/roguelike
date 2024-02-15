#ifndef _20240215012142
#define _20240215012142

#include "raylib.h"
#include "raylibEx.h"

Vector2Int screenToPosition(const Vector2& pixel);
Vector2 positionToWorld(const Vector2Int& position);

void setMouseTile(Vector2Int& position);

#endif
