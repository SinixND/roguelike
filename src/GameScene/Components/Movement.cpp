#include "Movement.h"

#include "Position.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void Movement::trigger(Vector2I const& direction)
{
    currentVelocity_ = Vector2Scale(direction, (speed_ * TileData::TILE_SIZE));
};

void Movement::update(Position& position)
{
    Vector2 distance{Vector2Scale(currentVelocity_, GetFrameTime())};
    float length{Vector2Length(distance)};

    cumulativeDistanceMoved_ += length;

    // If movement not completed
    if (cumulativeDistanceMoved_ < TileData::TILE_SIZE)
    {
        position.move(distance);
        return;
    }

    // End movement (set currentVelocity = {0,0}) if moved more than or equal to one tile
    currentVelocity_ = Vector2{0, 0};
    // Move by remaining distance unitl TILE_SIZE
    position.move(Vector2ClampValue(distance, 0, TileData::TILE_SIZE - (cumulativeDistanceMoved_ - length)));
    // Reset cumulativeDistanceMoved
    cumulativeDistanceMoved_ = 0;
};