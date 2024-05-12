#include "RayCast.h"

#include "RotationMatrices.h"
#include "SparseSet.h"
#include "TextureData.h"
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
    void ensureNonZeroValues(Vector2& directionTransformed)
    {
        if (abs(directionTransformed.x) < EPSILON)
        {
            directionTransformed.x = EPSILON;
        }

        if (abs(directionTransformed.y) < EPSILON)
        {
            directionTransformed.y = (EPSILON);
        }
    }

    void handleEqualIncrements(
        Vector2& incrementedRayLengths,
        Vector2 rayLengthIncrementsInDirection,
        float& maxLength)
    {
        if (FloatEquals(rayLengthIncrementsInDirection.x, rayLengthIncrementsInDirection.y))
        {
            incrementedRayLengths.y = maxLength;
        }
    }

    void updateShorterLength(
        Vector2 rayLengthIncrementsInDirection,
        Vector2& incrementedRayLengths,
        float& shorterLength)
    {
        if (incrementedRayLengths.x < incrementedRayLengths.y)
        {
            // Cache shorterLength
            shorterLength = incrementedRayLengths.x;

            // Update ray length
            incrementedRayLengths.x += rayLengthIncrementsInDirection.x;
        }
        else
        {
            // Cache shorterLength
            shorterLength = incrementedRayLengths.y;

            // Update ray length
            incrementedRayLengths.y += rayLengthIncrementsInDirection.y;
        }
    }

    void checkIfTileCornerHit(
        Vector2 const& rayEnd,
        bool& isCornerHit)
    {
        Vector2 rayEndDistancesFromTileCorner{
            remainder((rayEnd.x + (TextureData::TILE_SIZE / 2)), TextureData::TILE_SIZE),
            remainder((rayEnd.y + (TextureData::TILE_SIZE / 2)), TextureData::TILE_SIZE)};

        if ((abs(rayEndDistancesFromTileCorner.x) < RAY_CAST_PRECISION) && (abs(rayEndDistancesFromTileCorner.y) < RAY_CAST_PRECISION))
        {
            isCornerHit = true;
        }
    }

    void updaterayEndOffsetsToCheck(
        std::vector<Vector2>& rayEndOffsetsToCheck,
        bool const& isCornerHit)
    {
        if (isCornerHit)
        {
            rayEndOffsetsToCheck.pop_back();
            // Add offsets for 4 adjacent tiles
            rayEndOffsetsToCheck.push_back({-RAY_CAST_PRECISION, -RAY_CAST_PRECISION});
            rayEndOffsetsToCheck.push_back({RAY_CAST_PRECISION, -RAY_CAST_PRECISION});
            rayEndOffsetsToCheck.push_back({-RAY_CAST_PRECISION, RAY_CAST_PRECISION});
            rayEndOffsetsToCheck.push_back({RAY_CAST_PRECISION, RAY_CAST_PRECISION});
        }
    }

    void processrayEndOffsetsToCheck(
        snx::SparseSet<Vector2I, Tile*>& tilesPassedByRay,
        TileMap& tileMap,
        Vector2& rayEnd,
        bool& isCornerHit,
        std::vector<Vector2>& rayEndOffsetsToCheck,
        bool& collisionDetected)
    {
        for (auto offset : rayEndOffsetsToCheck)
        {
            Vector2 newRayEndToCheck = Vector2Add(
                rayEnd,
                offset);

            Tile* tileHit{tileMap.at(TileTransformation::worldToPosition(newRayEndToCheck))};

            // Check for nullptr aka. tile out of map
            if (!tileHit)
            {
                continue;
            }

            // Corners are not considered part of a tile which can be passed by ray
            if (!isCornerHit)
            {
                tilesPassedByRay.createOrUpdate(tileHit->positionComponent.tilePosition(), tileHit);
            }

            // Detect tile blocking vision
            if (tileHit->blocksVision())
            {
                collisionDetected = true;
                break;
            }
        }
    }

    void processCurrentRay(
        snx::SparseSet<Vector2I, Tile*>& tilesPassedByRay,
        Vector2 origin,
        TileMap& tileMap,
        Vector2 rayDirection,
        float maxLength,
        Vector2 rayLengthIncrementsInDirection,
        Vector2& incrementedRayLengths,
        Vector2& rayEnd,
        float& shorterLength)
    {
        while (true)
        {
            // udpate shorterLength
            updateShorterLength(
                rayLengthIncrementsInDirection,
                incrementedRayLengths,
                shorterLength);

            if (shorterLength > (maxLength + 1))
            {
                break;
            }

            // If rayLenghts are equal increase one once more to avoid double checks
            if (FloatEquals(incrementedRayLengths.x, incrementedRayLengths.y))
            {
                incrementedRayLengths.x += rayLengthIncrementsInDirection.x;
            }

            // Update target tile position
            rayEnd = Vector2Add(
                origin,
                Vector2Scale(
                    rayDirection,
                    shorterLength));

            // Tile corners
            // If tile corner hit check 4 adjacent tiles
            bool isCornerHit{false};

            // Offset from corner
            checkIfTileCornerHit(rayEnd, isCornerHit);

            std::vector<Vector2> rayEndOffsetsToCheck{{0, 0}};

            updaterayEndOffsetsToCheck(rayEndOffsetsToCheck, isCornerHit);

            bool collisionDetected{false};

            processrayEndOffsetsToCheck(
                tilesPassedByRay,
                tileMap,
                rayEnd,
                isCornerHit,
                rayEndOffsetsToCheck,
                collisionDetected);

            if (collisionDetected)
            {
                break;
            }
        }
    }

    void processRayTargets(
        snx::SparseSet<Vector2I, Tile*>& tilesPassedByRay,
        std::vector<Vector2I> const& rayTargets,
        Vector2 origin,
        TileMap& tileMap,
        float unitRelative)
    {
        for (auto const& rayTarget : rayTargets)
        {
            Vector2 target{TileTransformation::positionToWorld(rayTarget)};

            Vector2 ray{Vector2Subtract(target, origin)};
            Vector2 rayDirection{Vector2Normalize(ray)};
            float maxLength{Vector2Length(ray)};

            // Rotation of rayDirection (= step one of transformation from relative to reference)
            Vector2 rayDirectionTransformed{
                Vector2Transform(
                    RotationMatrices::M_ROTATE_CW_45,
                    rayDirection)};

            // Prevent division by 0: if < precision then set to precision
            // Introduces a small amount of error, but should be insignificant
            ensureNonZeroValues(rayDirectionTransformed);

            // Normalize transformed rayDirection to relative unit (= rayIncrements)
            Vector2 const rayLengthIncrementsInDirection{
                sqrtf(pow(unitRelative, 2) * (1 + pow((rayDirectionTransformed.y / rayDirectionTransformed.x), 2))),
                sqrtf(pow(unitRelative, 2) * (1 + pow((rayDirectionTransformed.x / rayDirectionTransformed.y), 2)))};

            Vector2 incrementedRayLengths{};

            // If both increments are equal, set one incrementedRayLength to maxLength to avoid double checks
            handleEqualIncrements(
                incrementedRayLengths,
                rayLengthIncrementsInDirection,
                maxLength);

            Vector2 rayEnd{origin};

            // Incrementing rayLength by smaller increment (result: shorterLength) until maxLength
            float shorterLength{
                (rayLengthIncrementsInDirection.x < rayLengthIncrementsInDirection.y)
                    ? rayLengthIncrementsInDirection.x
                    : rayLengthIncrementsInDirection.y};

            processCurrentRay(
                tilesPassedByRay,
                origin,
                tileMap,
                rayDirection,
                maxLength,
                rayLengthIncrementsInDirection,
                incrementedRayLengths,
                rayEnd,
                shorterLength);
        }
    }

    auto getTilesPassedByRay(
        std::vector<Vector2I>& rayTargets,
        Vector2 origin,
        TileMap& tileMap) -> std::vector<Tile*>
    {
        snx::SparseSet<Vector2I, Tile*> tilesPassedByRay{};

        /* ALGORITHM EXPLANATION
        To get the intersections with tile diagonals (which form a relative grid coordinate system (aka. csys)) we need to transform the direction vector.

        Because we can only easily calculate within our reference (base) csys (aka. "frame of reference"), we transform FROM THE RELATIVE TO the REFERENCE csys.

        That requires a ROTATION by 45 deg (CW or CCW, does not matter), and (see next step) a SCALING FACTOR of SQRT(2) (reference unit (TextureData::TILE_SIZE) is the hypothenuse in the relative normalized unit csys).

        To get the ray increment (-> hypothenuse, for x = 1 reference unit and y = 1 reference unit) back to the relative system, we need to NORMALIZE (both for the "x" and "y" component) the hypothenuse / length to the relative systems unit, which is our reference unit DOWNSCALED.
        */

        float unitRelative{TextureData::TILE_SIZE / sqrt(2.0F)};

        processRayTargets(
            tilesPassedByRay,
            rayTargets,
            origin,
            tileMap,
            unitRelative);

        return tilesPassedByRay.values();
    }

    auto getTilesPassedByRay(
        std::vector<Vector2I>& rayTargets,
        Vector2I origin,
        TileMap& tileMap) -> std::vector<Tile*>
    {
        return getTilesPassedByRay(rayTargets, TileTransformation::positionToWorld(origin), tileMap);
    }
}
