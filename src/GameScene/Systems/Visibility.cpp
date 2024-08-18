#include "Visibility.h"

#include "Debugger.h"
#include "TileData.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cassert>
#include <cstddef>
#include <raylib.h>
#include <string>
#include <vector>

// Visibility::Shadow

Visibility::Shadow::Shadow(Vector2I const& octantPosition)
{
    setSlopeLeft(octantPosition);
    setSlopeRight(octantPosition);
}

void Visibility::Shadow::setSlopeLeft(Vector2I const& octantPosition)
{
    slopeLeft_ = (octantPosition.y + 0.5f) / (octantPosition.x - 0.5f);
}

void Visibility::Shadow::setSlopeLeft(float slopeLeft)
{
    slopeLeft_ = slopeLeft;
}

void Visibility::Shadow::setSlopeRight(Vector2I const& octantPosition)
{
    slopeRight_ = (octantPosition.y - 0.5f) / (octantPosition.x + 0.5f);
}

void Visibility::Shadow::setSlopeRight(float slopeRight)
{
    slopeRight_ = slopeRight;
}

float Visibility::Shadow::getLeftAtTop(Vector2I const& octantPosition)
{
    return (octantPosition.y + 0.5f) / slopeLeft_;
}

float Visibility::Shadow::getLeft(int octantPositionHeight)
{
    return (octantPositionHeight) / slopeLeft_;
}

float Visibility::Shadow::getRightAtBottom(Vector2I const& octantPosition)
{
    return (octantPosition.y - 0.5f) / slopeRight_;
}

float Visibility::Shadow::getRight(int octantPositionHeight)
{
    return (octantPositionHeight) / slopeRight_;
}

// Visibility

void Visibility::addShadow(
    std::vector<Shadow>& shadowLine,
    Vector2I const& octantPosition)
{
    Shadow shadowNew{octantPosition};

    float shadowNewLeft{shadowNew.getLeftAtTop(octantPosition)};
    float shadowNewRight{shadowNew.getRightAtBottom(octantPosition)};

    size_t i{0};
    int shadowContainingLeftEnd{-1};
    int shadowContainingRightEnd{-1};

    for (; i < shadowLine.size(); ++i)
    {
        //          ||__old_
        // -new--|          
        if (shadowNewRight < shadowLine[i].getLeftAtTop(octantPosition))
        {
            // New ends before current
            continue;
        }

        // _old__||
        //           |--new-
        if (shadowLine[i].getRightAtBottom(octantPosition) < shadowNewLeft)
        {
            // Current ends before new
            continue;
        }

        // ||__old_  _old__||
        //  |--new-  -new--|
        if (
            shadowLine[i].getLeftAtTop(octantPosition) <= shadowNewLeft
            && shadowNewRight <= shadowLine[i].getRightAtBottom(octantPosition))
        {
            // New is contained
            break;
        }

        //     ||__old_  _old__||
        // |--new-       -new--|
        if (
            shadowNewLeft < shadowLine[i].getLeftAtTop(octantPosition)
            && shadowNewRight <= shadowLine[i].getRightAtBottom(octantPosition))
        {
            if (shadowContainingRightEnd > -1)
            {
                // Merge shadows
                // Adjust remaining
                shadowLine[shadowContainingRightEnd].setSlopeLeft(shadowNew.slopeLeft());
                // Delete consumed
                shadowLine.erase(shadowLine.begin() + i);
                break;
            }

            // Extend old to left
            shadowLine[i].setSlopeLeft(shadowNew.slopeLeft());
            shadowContainingLeftEnd = i;
        }

        // ||__old_  _old__||
        //  |--new-      -new--|
        if (
            shadowLine[i].getLeftAtTop(octantPosition) <= shadowNewLeft
            && shadowLine[i].getRightAtBottom(octantPosition) < shadowNewRight)
        {
            if (shadowContainingLeftEnd > -1)
            {
                // Merge shadows
                // Adjust remaining
                shadowLine[shadowContainingLeftEnd].setSlopeRight(shadowNew.slopeRight());
                // Delete consumed
                shadowLine.erase(shadowLine.begin() + i);
                break;
            }

            // Extend old to right
            shadowLine[i].setSlopeRight(shadowNew.slopeRight());
            shadowContainingRightEnd = i;
        }

        // Note: No case where new is covering old possible
    }

    if ((shadowContainingLeftEnd + shadowContainingRightEnd) < 0)
    {
        // No intersections, add new shadow
        shadowLine.push_back(shadowNew);
    }
}

void Visibility::calculateOctant(int octant, Tiles& map, Vector2I const& heroPosition, int range)
{
    // 2nd octant row(y = 1) is visible -> neighbour tiles (N) to hero (H)
    // !!! Coordinates are usual cartesian !!!
    //
    //   0 1 2
    // 2 - - - // <- shadowline(s)
    // 1 N N
    // 0 H
#ifdef DEBUG
    // BeginDrawing();
    // BeginMode2D(snx::dbg::cam());
    // DrawRectangleV(
    //     Vector2SubtractValue(
    //         UnitConversion::tileToWorld(heroPosition),
    //         TileData::TILE_SIZE_HALF),
    //     TileData::TILE_DIMENSIONS,
    //     GREEN);
    // EndMode2D();
    // EndDrawing();
#endif

    std::vector<Shadow> shadowLine{};

    // Iterate octant, start at row 2 (y = 1)
    for (int octY{1}; octY < range; ++octY)
    {
        for (int octX{0}; octX <= octY; ++octX)
        {
#ifdef DEBUG
            // BeginDrawing();
            // BeginMode2D(snx::dbg::cam());
            // DrawCircleLinesV(UnitConversion::tileToWorld(UnitConversion::octantToTile({octX, octY}, octant, heroPosition)), 3, WHITE);
            // EndMode2D();
            // EndDrawing();
#endif
            // Check visibility by intersection with shadow line elements
            bool isVisible{true};

            for (Shadow& shadow : shadowLine)
            {
#ifdef DEBUG
                // BeginDrawing();
                // BeginMode2D(snx::dbg::cam());
                // DrawLineEx({shadow.getLeft({int(octX * TileData::TILE_SIZE), int(octY * TileData::TILE_SIZE)}), octY * TileData::TILE_SIZE + 0.5f * TileData::TILE_SIZE}, {shadow.getRight({int(octX * TileData::TILE_SIZE), int(octY * TileData::TILE_SIZE)}), octY * TileData::TILE_SIZE + 0.5f * TileData::TILE_SIZE}, 2, RED);
                // EndMode2D();
                // EndDrawing();
#endif
                // NOTE: x = y / m
                // If top-left tile corner is left (<) from slopeLeft (at same hight = tileTop)
                if ((octX /*- 0.5f*/) < (shadow.getLeft(octY)))
                {
                    // top-left corner not in shadow -> visible
                    continue;
                }

                // If slopeRight is left (<) from bottom-right tile corner (at same hight = tileBottom)
                if ((shadow.getRight(octY)) < (octX /*+ 0.5f*/))
                {
                    // bottom-right corner not in shadow -> visible
                    continue;
                }

                // both corners in shadow -> invis
                isVisible = false;
                break;
            }

            VisibilityID tileVisiblityOld{
                map.visibilityID(
                    UnitConversion::octantToTile(
                        Vector2I{
                            octX,
                            octY},
                        octant,
                        heroPosition))};

            // Update visibility
            if (isVisible)
            {
                map.setVisibilityID(
                    UnitConversion::octantToTile(
                        Vector2I{
                            octX,
                            octY},
                        octant,
                        heroPosition),
                    VisibilityID::visible);
            }
            else
            {
                if (tileVisiblityOld == VisibilityID::visible)
                {
                    map.setVisibilityID(
                        UnitConversion::octantToTile(
                            Vector2I{
                                octX,
                                octY},
                            octant,
                            heroPosition),
                        VisibilityID::seen);
                }
            }

            // Update shadows
            if (map.blocksVision(UnitConversion::octantToTile(
                    Vector2I{octX, octY},
                    octant,
                    heroPosition)))
            {
                addShadow(shadowLine, Vector2I{octX, octY});
            }
        }
    }
}
void Visibility::update(
    Tiles& map,
    RectangleEx const& mapPanel,
    Vector2I const& heroPosition)
{
    // Input
    int rangeX{1 + static_cast<int>((mapPanel.width() / 2) / TileData::TILE_SIZE)};
    int rangeY{1 + static_cast<int>((mapPanel.height() / 2) / TileData::TILE_SIZE)};

    // Orientation dependent range (horizontal, vertical)
    int range{};

    // Init
    map.setVisibilityID(
        heroPosition,
        VisibilityID::visible);

    // Iterate octants
    for (int octant{0}; octant <= 7; ++octant)
    {
        // Set range for octant
        if (((octant + 1) / 2) % 2) // := f tt ff tt f
        {
            range = rangeX;
        }
        else
        {
            range = rangeY;
        }

        calculateOctant(octant, map, heroPosition, range);
    }
}
