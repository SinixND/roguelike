#include "Movement.h"

#include "Constants.h"
#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

bool Movement::move(Vector2& position, float dt)
{
    // Fraction of one tile size for path progressing
    static float tileFraction{};

    // Distance moved this frame
    float frameDistance = (speed * TILE_SIZE) * dt;

    // Update distances
    tileFraction += frameDistance;

    // Move
    // Update position: move in direction provided by path
    position = Vector2Add(position, Vector2Scale(path_.back().directionAccessed, frameDistance));

    // Progress path
    if (tileFraction > TILE_SIZE)
    {
        tileFraction = 0;
        position = TileTransformation::positionToWorld(path_.back().tile->position.tilePosition());
        path_.pop_back();
    }

    // Check if target reached
    if (path_.empty())
    {
        isMoving_ = false;
        return true;
    }

    return false;
}