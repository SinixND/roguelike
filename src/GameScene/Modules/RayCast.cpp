#include "RayCast.h"

#include "RotationMatrices.h"
#include "SparseSet.h"
#include "Textures.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileTransformation.h"
#include "raylibEx.h"
#include <cmath>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>

namespace RayCast
{
    auto getTilesRayed(
        std::vector<Vector2I>& rayTargets,
        Vector2 origin,
        TileMap& tileMap) -> std::vector<Tile*>
    {
        snx::SparseSet<Vector2I, Tile*> tilesRayed{};

        // Set ray cast values
        Vector2 rayStart{origin};

        float unitRelative{Textures::TILE_SIZE / sqrt(2.0F)};

        for (auto const& rayTarget : rayTargets)
        {
            Vector2 target{TileTransformation::positionToWorld(rayTarget)};

            Vector2 ray{Vector2Subtract(target, rayStart)};
            Vector2 rayDirection{Vector2Normalize(ray)};
            float maxLength{Vector2Length(ray)};

            /* ALGORITHM EXPLANATION
            To get the intersections with tile diagonals (which form a relative grid coordinate system (aka. csys)) we need to transform the direction vector.

            Because we can only easily calculate within our reference (base) csys (aka. "frame of reference"), we transform FROM THE RELATIVE TO the REFERENCE csys.

            That requires a ROTATION by 45 deg (CW or CCW, does not matter), and (see next step) a SCALING FACTOR of SQRT(2) (reference unit (Textures::TILE_SIZE) is the hypothenuse in the relative normalized unit csys).

            To get the ray increment (-> hypothenuse, for x = 1 reference unit and y = 1 reference unit) back to the relative system, we need to NORMALIZE (both for the "x" and "y" component) the hypothenuse / length to the relative systems unit, which is our reference unit DOWNSCALED.
            */

            // Rotation
            Vector2 directionTransformed{
                Vector2Transform(RotationMatrices::M_ROTATE_CW_45, rayDirection)};

            // Prevent division by 0: if < precision then set to precision
            // Introduces a small amount of error, but should be insignificant
            if (abs(directionTransformed.x) < EPSILON)
            {
                directionTransformed.x = EPSILON;
            }

            if (abs(directionTransformed.y) < EPSILON)
            {
                directionTransformed.y = (EPSILON);
            }

            // Normalize rayIncrements to relative unit
            Vector2 const rayLengthIncrements{
                sqrtf(pow(unitRelative, 2) * (1 + pow((directionTransformed.y / directionTransformed.x), 2))),
                sqrtf(pow(unitRelative, 2) * (1 + pow((directionTransformed.x / directionTransformed.y), 2)))};

            // Increment rayLength for shorter length until maxLength
            Vector2 rayLengths{};

            if (FloatEquals(rayLengthIncrements.x, rayLengthIncrements.y))
            {
                rayLengths.y = maxLength;
            }

            Vector2 rayEnd{rayStart};

            // ray incrementing loop
            float minLength{
                (rayLengthIncrements.x < rayLengthIncrements.y)
                    ? rayLengthIncrements.x
                    : rayLengthIncrements.y};

            while (true)
            {
                // udpate minLength
                if (rayLengths.x < rayLengths.y)
                {
                    // Cache minLength
                    minLength = rayLengths.x;

                    // Update ray length
                    rayLengths.x += rayLengthIncrements.x;
                }
                else
                {
                    // Cache position
                    minLength = rayLengths.y;

                    // Update ray length
                    rayLengths.y += rayLengthIncrements.y;
                }

                if (minLength > (maxLength + 1))
                {
                    break;
                }

                // If rayLenghts are equal increase one once more to avoid double checks
                if (FloatEquals(rayLengths.x, rayLengths.y))
                {
                    rayLengths.x += rayLengthIncrements.x;
                }

                // Update target tile position
                rayEnd = Vector2Add(
                    rayStart,
                    Vector2Scale(
                        rayDirection,
                        minLength));

                // Tile corners
                // If tile corner hit check 4 adjacent tiles

                // Offset from corner
                float cornerOffsetX{remainder((rayEnd.x + (Textures::TILE_SIZE / 2)), Textures::TILE_SIZE)};
                float cornerOffsetY{remainder((rayEnd.y + (Textures::TILE_SIZE / 2)), Textures::TILE_SIZE)};

                bool cornerHit{false};

                if ((abs(cornerOffsetX) < RAY_CAST_PRECISION) && (abs(cornerOffsetY) < RAY_CAST_PRECISION))
                {
                    cornerHit = true;
                }

                std::vector<Vector2> offsets{{0, 0}};

                if (cornerHit)
                {
                    offsets.pop_back();
                    // Add offsets for 4 adjacent tiles
                    offsets.push_back({-RAY_CAST_PRECISION, -RAY_CAST_PRECISION});
                    offsets.push_back({RAY_CAST_PRECISION, -RAY_CAST_PRECISION});
                    offsets.push_back({-RAY_CAST_PRECISION, RAY_CAST_PRECISION});
                    offsets.push_back({RAY_CAST_PRECISION, RAY_CAST_PRECISION});
                }

                bool collisionDetected{false};

                for (auto offset : offsets)
                {
                    Vector2 offsetRayEnd = rayEnd;

                    offsetRayEnd += offset;

                    Tile* tileHit{tileMap.at(TileTransformation::worldToPosition(offsetRayEnd))};

                    // Check for nullptr aka. tile out of map
                    if (!tileHit)
                    {
                        continue;
                    }

                    if (!cornerHit)
                    {
                        tilesRayed.createOrUpdate(tileHit->positionComponent.tilePosition(), tileHit);
                    }

                    // Detect tile blocking vision
                    if (tileHit->blocksVision())
                    {
                        collisionDetected = true;
                        break;
                    }
                }

                if (collisionDetected)
                {
                    break;
                }
            }
        }

        return tilesRayed.values();
    }

    auto getTilesRayed(
        std::vector<Vector2I>& rayTargets,
        Vector2I origin,
        TileMap& tileMap) -> std::vector<Tile*>
    {
        return getTilesRayed(rayTargets, TileTransformation::positionToWorld(origin), tileMap);
    }
}
