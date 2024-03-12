#include "RayCast.h"

#include "Constants.h"
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
    std::vector<Tile*> getTilesRayed(std::vector<Vector2i>& rayTargets, const Vector2& origin, TileMap& tileMap)
    {
        std::vector<Tile*> tilesRayed{};

        // Set ray cast values
        Vector2 rayStart{origin};

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
            bool visionBlocked{false};
            float minLength{(rayLengthIncrements.x < rayLengthIncrements.y) ? rayLengthIncrements.x : rayLengthIncrements.y};

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

                // Ignore edge cases
                //* int x{static_cast<int>(std::round((rayEnd.x / TILE_SIZE) * 10)) % 10};
                //* int y{static_cast<int>(std::round((rayEnd.y / TILE_SIZE) * 10)) % 10};
                //* if ((x == 5) || (y == 5))
                //* continue;

                Tile* tileHit{tileMap.at(TileTransformation::worldToPosition(rayEnd))};

                // Check for nullptr aka. tile out of map
                if (!tileHit)
                    continue;

                // Detect tile blocking vision
                if (tileHit->blocksVision)
                    visionBlocked = true;

                // End ray cast if vision blocking tile is passed
                if (!tileHit->blocksVision && visionBlocked)
                    break;

                // Add rayed tile
                tilesRayed.push_back(tileHit);

                //* if (dtb::debugMode())
                //* {
                //* BeginDrawing();
                //* BeginMode2D(dtb::camera());

                //* DrawLineEx(rayStart, rayEnd, 1, GREEN);
                //* DrawCircleLinesV(tileHit->position.get(), 3, WHITE);

                //* EndMode2D();
                //* EndDrawing();
                //* }
            }
        }

        return tilesRayed;
    }
}
