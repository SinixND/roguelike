#include "Movement.h"

#include "Textures.h"
#include "TileTransformation.h"
#include "Transformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

auto Movement::move(Transformation& transform, float dt) -> bool
{
    // Fraction of one tile size for path progressing
    static float tileFraction{};

    // Distance moved this frame
    float frameDistance = (speed_ * Textures::TILE_SIZE) * dt;

    // Update distances
    tileFraction += frameDistance;

    // Move
    // Update position: move in direction provided by path
    transform.setPosition(
        Vector2Add(
            transform.position(),
            Vector2Scale(
                path_.back().directionAccessed,
                frameDistance)));

    // Progress path
    if (tileFraction > Textures::TILE_SIZE)
    {
        tileFraction = 0;
        transform.setPosition(TileTransformation::positionToWorld(path_.back().tile->transform.tilePosition()));
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