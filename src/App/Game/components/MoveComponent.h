#ifndef IG20250421002743
#define IG20250421002743

#include "EnergyComponent.h"
#include "TileData.h"
#include "raylibEx.h"

/// Highest visual speed
float constexpr SPEED_MAX{ 20 * TileData::TILE_SIZE };

/// Helper values
float const SPEED_MIN{ SPEED_MAX / SPEED_RATIO_MAX };

struct MoveComponent
{
    Vector2I direction{};
    float speed{};
    float remainingDistance{};
};

#endif
