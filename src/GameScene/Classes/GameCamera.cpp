#include "GameCamera.h"

#include <raylib.h>

void GameCamera::init(Vector2 const& offset)
{
    camera_ = Camera2D{
        offset,
        Vector2{0, 0},
        0,
        1};
}