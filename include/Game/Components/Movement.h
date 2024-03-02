#ifndef _20240218231113
#define _20240218231113

#include "Constants.h"
#include "Pathfinder.h"
#include "TilePosition.h"
#include "TilePositionConversion.h"
#include <cstddef>
#include <iostream>
#include <raymath.h>

class Movement
{
public:
    size_t range{};
    float speed{}; // pixel per s

public:
    TilePosition& target() { return target_; };
    void setTarget(const TilePosition& target)
    {
        target_ = target;
    };

    bool isTargetSet() { return targetSet_; };
    void setIsTargetSet(bool status) { targetSet_ = status; };

    bool isMoving() { return isMoving_; };
    //* void setIsMoving(bool state) { isMoving_ = state; };

    bool isExhausted() { return isExhausted_; };
    void setIsExhausted(bool state) { isExhausted_ = state; };

    void setPath(const Path& path)
    {
        path_ = path;
        isMoving_ = true;
    };

    // Returns if movement finished
    bool move(Vector2& position, float dt)
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
    };

private:
    TilePosition target_{};
    bool targetSet_{false};
    bool isMoving_{false};
    bool isExhausted_{false};
    Path path_{};
};

#endif