#include "Vision.h"

#include "Constants.h"
#include "RuntimeDatabase.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileMapFilters.h"
#include "TileTransformation.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cmath>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>

namespace Vision
{
    void update(Unit& unit, TileMap& tileMap)
    {
        // Filter tiles
        std::vector<Vector2i> tilesInExtendedVisionRange{
            TileMapFilters::filterInRange(
                tileMap,
                unit.visionRange + 1,
                unit.position.tilePosition())};

        std::vector<Vector2i> tilesOutOfVisionRange{
            TileMapFilters::filterInRange(
                tilesInExtendedVisionRange,
                unit.visionRange + 1,
                unit.visionRange + 1,
                unit.position.tilePosition())};

        std::vector<Vector2i> tilesInVisionRange{
            TileMapFilters::filterInRange(
                tilesInExtendedVisionRange,
                unit.visionRange,
                unit.position.tilePosition())};

        std::vector<Vector2i> rayTargets{
            TileMapFilters::filterInRange(
                tilesInVisionRange,
                unit.visionRange,
                unit.visionRange,
                unit.position.tilePosition())};

        int visionRange{unit.visionRange};
        int targetCount{4 * unit.visionRange};
        while (static_cast<int>(rayTargets.size()) < (targetCount))
        {
            --visionRange;

            targetCount = rayTargets.size();

            for (const Vector2i& position : TileMapFilters::filterInRange(tilesInVisionRange, visionRange, visionRange, unit.position.tilePosition()))
            {
                rayTargets.push_back(position);
            }

            targetCount += (4 * visionRange);
        }

        // Reset "visible" tiles to "seen" outside of vision range
        for (auto& tilePosition : tilesOutOfVisionRange)
        {
            auto tile{tileMap.at(tilePosition)};

            if (tile->visibilityID == VisibilityID::visible)
            {
                tile->visibilityID = VisibilityID::seen;

                if (dtb::debugMode())
                {
                    BeginDrawing();
                    BeginMode2D(dtb::camera());

                    DrawCircleLinesV(tile->position.get(), 5, RED);

                    EndMode2D();
                    EndDrawing();
                }
            }
        }

        // Set ray cast values
        Vector2 rayStart{unit.position.get()};

        float unitRelative{TILE_SIZE / sqrt(2.0f)};

        for (auto& rayTarget : rayTargets)
        {
            Vector2 ray{
                Vector2Subtract(
                    TileTransformation::positionToWorld(rayTarget),
                    rayStart)};

            Vector2 rayDirection{Vector2Normalize(ray)};

            float maxLength{Vector2Length(ray)};

            /* ALGORITHM EXPLANATION
            //
            // To get the intersections with tile diagonals (which form a relative grid / coordinate system (aka. csys))
            // we need to transform the direction vector.
            //
            // Because we can only easily calculate within our reference (base) csys (aka. "frame of reference"),
            // we transform FROM THE RELATIVE TO the REFERENCE csys.
            //
            // That requires [1.] a ROTATION by 45 deg (CW or CCW, does not matter),
            // and (see next step) a SCALING FACTOR of SQRT(2) (reference unit (TILE_SIZE) is the hypothenuse in the relative normalized unit csys)
            //
            // To get the ray increment (-> hypothenuse, for x = 1 reference unit and y = 1 reference unit) back to the relative system,
            // we need to [2.] NORMALIZE (both for the "x" and "y" component) the hypothenuse / length to the relative systems unit,
            // which is our reference unit DOWNSCALED.
            */

            // [1.] Rotation
            Vector2 directionTransformed{Vector2Transform(M_ROTATE_CW_45, rayDirection)};

            // Prevent division by 0: if < precision then set to precision
            // Introduces a small amount of error, but should be insignificant
            if (abs(directionTransformed.x) < EPSILON)
                directionTransformed.x = EPSILON;

            if (abs(directionTransformed.y) < EPSILON)
                directionTransformed.y = (EPSILON);

            // [2.] Normalized rayIncrements
            const Vector2 rayLengthIncrements{
                sqrtf(pow(unitRelative, 2) * (1 + pow((directionTransformed.y / directionTransformed.x), 2))),
                sqrtf(pow(unitRelative, 2) * (1 + pow((directionTransformed.x / directionTransformed.y), 2)))};

            Vector2 rayLengths{};
            if (FloatEquals(rayLengthIncrements.x, rayLengthIncrements.y))
                rayLengths.y = maxLength;

            Vector2 rayEnd{rayStart};

            // Increment rayLength for shorter length until maxLength

            // ray incrementing loop
            bool tileFound{false};
            float minLength{(rayLengthIncrements.x < rayLengthIncrements.y) ? rayLengthIncrements.x : rayLengthIncrements.y};

            while (!tileFound)
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

                if (minLength > maxLength)
                    break;

                // If rayLenghts are equal increase one once more to avoid double checks
                if (FloatEquals(rayLengths.x, rayLengths.y))
                    rayLengths.x += rayLengthIncrements.x;

                // Update target tile position
                rayEnd = Vector2Add(
                    rayStart,
                    Vector2Scale(
                        rayDirection,
                        minLength));

                // Make current target visible
                Tile* tileHit{tileMap.at(TileTransformation::worldToPosition(rayEnd))};

                tileHit->visibilityID = VisibilityID::visible;

                if (dtb::debugMode())
                {
                    BeginDrawing();
                    BeginMode2D(dtb::camera());

                    DrawLineEx(rayStart, rayEnd, 1, GREEN);
                    DrawCircleLinesV(tileHit->position.get(), 3, WHITE);

                    EndMode2D();
                    EndDrawing();
                }

                // End ray cast if opaque is hit
                if (tileHit->blocksVision)
                    break;
            }
        }
    }
}