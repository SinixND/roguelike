#include "Movement.h"

bool Movement::move(Vector2& position, float dt)
{
    // Whole distance moved to compare against range
    static float wholeDistanceMoved{};

    // Fraction of one tile size for path progressing
    static float tileFraction{};

    // Distance moved this frame
    float frameDistance = (speed * TILE_SIZE) * dt;

    // Update distances
    wholeDistanceMoved += frameDistance;
    tileFraction += frameDistance;

    // Move
    // Update position: move in direction provided by path
    position = Vector2Add(position, Vector2Scale(path_.back().directionAccessed, frameDistance));

    // Progress path
    if (tileFraction > TILE_SIZE)
    {
        tileFraction = 0;
        wholeDistanceMoved -= TILE_SIZE - tileFraction;
        position = tilePositionToWorld(path_.back().tilePosition);
        path_.pop_back();

        // Check if target reached
        if (path_.empty())
        {
            isMoving_ = false;
            return true;
        }
    }

    if (wholeDistanceMoved >= (range * TILE_SIZE))
    {
        isMoving_ = false;
        isExhausted_ = true;
        return true;
    }

    return false;
}