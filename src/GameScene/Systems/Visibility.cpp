#include "Visibility.h"
// #define DEBUG_SHADOW
// #define DEBUG_FOG

#include "TileData.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#if defined(DEBUG) && defined(DEBUG_SHADOW)
#include "Debugger.h"
#endif

// Shadow
Shadow::Shadow(Vector2I const& octantPosition)
{
    setSlopeLeft(octantPosition);
    setSlopeRight(octantPosition);
}

void Shadow::setSlopeLeft(Vector2I const& octantPosition)
{
    slopeLeft_ = (octantPosition.y + 0.5f) / (octantPosition.x - 0.5f);
}

void Shadow::setSlopeLeft(float slopeLeft)
{
    slopeLeft_ = slopeLeft;
}

void Shadow::setSlopeRight(Vector2I const& octantPosition)
{
    // slopeRight_ = (octantPosition.y - 0.5f) / (octantPosition.x + 0.5f);
    slopeRight_ = std::max(EPSILON, (octantPosition.y - 0.5f) / (octantPosition.x + 0.5f));
}

void Shadow::setSlopeRight(float slopeRight)
{
    slopeRight_ = slopeRight;
}

float Shadow::getLeftAtTop(Vector2I const& octantPosition) const
{
    return (octantPosition.y + 0.5f) / slopeLeft_;
}

float Shadow::getLeftAtBottom(Vector2I const& octantPosition) const
{
    return (octantPosition.y - 0.5f) / slopeLeft_;
}

float Shadow::getLeft(int octantPositionHeight) const
{
    // NOTE: x = y / m
    return (octantPositionHeight) / slopeLeft_;
}

float Shadow::getRightAtTop(Vector2I const& octantPosition) const
{
    return (octantPosition.y + 0.5f) / slopeRight_;
}

float Shadow::getRightAtBottom(Vector2I const& octantPosition) const
{
    return (octantPosition.y - 0.5f) / slopeRight_;
}

float Shadow::getRight(int octantPositionHeight) const
{
    // NOTE: x = y / m
    return (octantPositionHeight) / slopeRight_;
}
#if defined(DEBUG) && defined(DEBUG_SHADOW)
void drawShadow(
    Shadow const& shadowNew,
    Vector2I const& octantPosition,
    int octant,
    Vector2I const& origin,
    float maxHeight)
{
    // BeginDrawing();

    // Draw left shadow projection
    DrawLineEx(
        UnitConversion::octantToScreen(
            Vector2{
                shadowNew.getLeftAtTop(octantPosition) * TileData::TILE_SIZE,
                (octantPosition.y * TileData::TILE_SIZE) + TileData::TILE_SIZE_HALF},
            octant,
            origin,
            snx::debug::gcam().camera()),
        UnitConversion::octantToScreen(
            Vector2{
                shadowNew.getLeft(maxHeight * TileData::TILE_SIZE),
                maxHeight * TileData::TILE_SIZE},
            octant,
            origin,
            snx::debug::gcam().camera()),
        1,
        YELLOW);

    // Draw right shadow projection
    DrawLineEx(
        UnitConversion::octantToScreen(
            Vector2{
                shadowNew.getRightAtBottom(octantPosition) * TileData::TILE_SIZE,
                (octantPosition.y * TileData::TILE_SIZE) - TileData::TILE_SIZE_HALF},
            octant,
            origin,
            snx::debug::gcam().camera()),
        UnitConversion::octantToScreen(
            Vector2{
                shadowNew.getRight(maxHeight * TileData::TILE_SIZE),
                maxHeight * TileData::TILE_SIZE},
            octant,
            origin,
            snx::debug::gcam().camera()),
        1,
        YELLOW);

    // EndDrawing();
}
#endif

// Visibility
void Visibility::updateShadowline(
    std::vector<Shadow>& shadowline,
    Vector2I const& octantPosition)
{
    Shadow shadowNew{octantPosition};

    float shadowNewLeftAtTop{shadowNew.getLeftAtTop(octantPosition)};
    float shadowNewRightAtBottom{shadowNew.getRightAtBottom(octantPosition)};

    int shadowContainingLeftEnd{-1};
    int shadowContainingRightEnd{-1};

    for (size_t i{0}; i < shadowline.size(); ++i)
    {
#if defined(DEBUG) && defined(DEBUG_SHADOW)
        snx::debug::cliLog(
            "Compare new shadow(",
            shadowNew.slopeLeft(),
            ", ",
            shadowNew.slopeRight(),
            ") with old shadow[",
            i,
            "](",
            shadowline[i].slopeLeft(),
            ", ",
            shadowline[i].slopeRight(),
            "):\n");
#endif

        //           ||__old_
        // -new--|
        if (shadowNewRightAtBottom < shadowline[i].getLeftAtBottom(octantPosition))
        {
            // New ends before current
            continue;
        }

        // _old__||
        //            |--new-
        if (shadowline[i].getRightAtTop(octantPosition) < shadowNewLeftAtTop)
        {
            // Current ends before new
            continue;
        }

        // ||__old_   _old__||
        //  |--new-   -new--|
        if (
            shadowline[i].getLeftAtTop(octantPosition) <= shadowNewLeftAtTop
            && shadowNewRightAtBottom <= shadowline[i].getRightAtBottom(octantPosition))
        {
            // Old contains new
            shadowContainingLeftEnd = i;
            shadowContainingRightEnd = i;
            break;
        }

        //        ||__old_   _old__||
        // |--new-   -new--|
        if (
            shadowNewLeftAtTop < shadowline[i].getLeftAtTop(octantPosition)
            && shadowline[i].getLeftAtBottom(octantPosition) <= shadowNewRightAtBottom
            && shadowNewRightAtBottom <= shadowline[i].getRightAtBottom(octantPosition))
        {
            // Merge shadows if another shadow contains left end already
            if (shadowContainingLeftEnd > -1)
            {
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog("Merge: Extend shadow[", i, "] to the right\n");
#endif

                // Adjust remaining
                shadowline[shadowContainingLeftEnd].setSlopeRight(shadowline[i].slopeRight());
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog(
                    "New shadow is (",
                    shadowline[shadowContainingLeftEnd].slopeLeft(),
                    ", ",
                    shadowline[shadowContainingLeftEnd].slopeRight(),
                    ")\n");
#endif

                // Delete consumed
                shadowline.erase(shadowline.begin() + i);
                break;
            }

            // Extend old to left
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog("Extend left\n");
#endif

            shadowline[i].setSlopeLeft(shadowNew.slopeLeft());
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog(
                "New shadow is (",
                shadowline[i].slopeLeft(),
                ", ",
                shadowline[i].slopeRight(),
                ")\n");
#endif

            shadowContainingRightEnd = i;
        }

        // ||__old_   _old__||
        //          |--new-   -new--|
        if (
            shadowline[i].getLeftAtTop(octantPosition) <= shadowNewLeftAtTop
            && shadowNewLeftAtTop <= shadowline[i].getRightAtTop(octantPosition)
            && shadowline[i].getRightAtBottom(octantPosition) < shadowNewRightAtBottom)
        {
            // Merge shadows if another shadow contains right end already
            if (shadowContainingRightEnd > -1)
            {
                // Adjust remaining
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog("Merge: Extend shadow[", i, "] to the left\n");
#endif

                shadowline[shadowContainingRightEnd].setSlopeLeft(shadowline[i].slopeLeft());
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog(
                    "New shadow is (",
                    shadowline[shadowContainingLeftEnd].slopeLeft(),
                    ", ",
                    shadowline[shadowContainingLeftEnd].slopeRight(),
                    ")\n");
#endif

                // Delete consumed
                shadowline.erase(shadowline.begin() + i);
                break;
            }

            // Extend old to right
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog("Extend right\n");
#endif

            shadowline[i].setSlopeRight(shadowNew.slopeRight());
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog(
                "New shadow is (",
                shadowline[i].slopeLeft(),
                ", ",
                shadowline[i].slopeRight(),
                ")\n");
#endif

            shadowContainingLeftEnd = i;
        }
        // Note: No case where new is covering old possible as shadows always get narrower the further from hero
    }

    // If no intersection with new shadow
    if ((shadowContainingLeftEnd + shadowContainingRightEnd) < -1)
    {
        // Add new shadow to shadow line
#if defined(DEBUG) && defined(DEBUG_SHADOW)
        snx::debug::cliLog("Create new shadow\n");
#endif

        shadowline.push_back(shadowNew);
    }
}

void Visibility::calculateVisibilitiesInOctant(
    int octant,
    Tiles& map,
    Vector2I const& heroPosition,
    int range)
{
    // !!! Coordinates are usual cartesian !!!
    std::vector<Shadow> shadowline{};

#if defined(DEBUG) && defined(DEBUG_SHADOW)
    BeginDrawing();
#endif
    // Iterate octant
    for (int octY{0}; octY < range; ++octY)
    {
        for (int octX{0}; octX <= octY + 1; ++octX)
        {
            Vector2I octantPosition{octX, octY};

            Vector2I tilePosition{
                UnitConversion::octantToTile(
                    octantPosition,
                    octant,
                    heroPosition)};
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog(
                "octantPosition",
                octantPosition,
                ", TilePosition",
                tilePosition,
                "\n");
#endif

            VisibilityID tileVisiblityOld{map.visibilityID(tilePosition)};

            // < : Update only octant tiles including diagonal tiles (spare last row tile needed for correct diagonal visiblity)
            if (octX <= octY)
            {
                // Skip test (-> set invis) if shadowline already covers whole octant
                // isShadowMax();
                if (
                    shadowline.size()
                    && shadowline[0].slopeLeft() < 0
                    && shadowline[0].slopeRight() < 1)
                {
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                    snx::debug::cliLog(
                        "Shadow is max.",
                        "\n");
#endif

                    if (tileVisiblityOld == VisibilityID::visible)
                    {
                        // Tile WAS visible
                        map.setVisibilityID(
                            tilePosition,
                            VisibilityID::seen);

                        // Add non opaque fog
                        fogsToRender_[tilePosition] = Fog{tilePosition, false};
                    }
                    else if (tileVisiblityOld == VisibilityID::seen)
                    {
                        // Add non opaque fog
                        fogsToRender_[tilePosition] = Fog{tilePosition, false};
                    }
                    else
                    {
                        // Add opaque fog
                        fogsToRender_[tilePosition] = Fog{tilePosition, true};
                    }

                    continue;
                } // Max shadow

                // Check visibility by intersection with shadow line elements
                bool isVisible{true};

                for (Shadow& shadow : shadowline)
                {
                    // If top-left tile corner is left (<) from slopeLeft (at same height = tileTop)
                    // OR
                    // If slopeRight is left (<) from bottom-right tile corner (at same height = tileBottom)
                    if ((octX - 0.5f) < (shadow.getLeftAtTop(octantPosition))
                        || (shadow.getRightAtBottom(octantPosition)) < (octX + 0.5f))
                    {
                        // top-left/bottom-right corner not in shadow -> visible (variable unchanged)
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                        snx::debug::cliLog(
                            "Tile is (partly) visible",
                            "\n");
#endif

                        continue;
                    }

                    isVisible = false;
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                    snx::debug::cliLog(
                        "Tile is invisible",
                        "\n");
#endif

                    break;
                } // Shadowline

                // Update visibility
                if (isVisible)
                {
                    // Tile IS visible
                    map.setVisibilityID(
                        tilePosition,
                        VisibilityID::visible);
                }
                else if (tileVisiblityOld == VisibilityID::visible)
                {
                    // Tile WAS visible
                    map.setVisibilityID(
                        tilePosition,
                        VisibilityID::seen);

                    // Add non opaque fog
                    fogsToRender_[tilePosition] = Fog{tilePosition, false};
                }
                else if (tileVisiblityOld == VisibilityID::seen)
                {
                    // Add non opaque fog
                    fogsToRender_[tilePosition] = Fog{tilePosition, false};
                }
                else
                {
                    // Add opaque fog
                    fogsToRender_[tilePosition] = Fog{tilePosition, true};
                }
            } // Octant tiles only

            // Update shadow line
            // updateShadowline(map, tilePosition);
            if (!map.isOpaque(tilePosition))
            {
                continue;
            }
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog(
                "Tile is opaque, add shadow...",
                "\n");

            drawShadow(
                Shadow{octantPosition},
                octantPosition,
                octant,
                heroPosition,
                range);
#endif

            updateShadowline(shadowline, octantPosition);
        } // Octant column
    } // Octant row
#if defined(DEBUG) && defined(DEBUG_SHADOW)
    EndDrawing();
#endif
}

void Visibility::update(
    Tiles& map,
    RectangleEx const& viewport,
    Vector2I const& heroPosition)
{
    // Input
    int octantWidth{2 + static_cast<int>((viewport.width() / 2) / TileData::TILE_SIZE)};
    int octantHeight{2 + static_cast<int>((viewport.height() / 2) / TileData::TILE_SIZE)};

    // Init
    fogsToRender_.clear();

    map.setVisibilityID(
        heroPosition,
        VisibilityID::visible);

    // Iterate octants
    // Orientation dependent range (horizontal, vertical)
    int range{};

    for (int octant{0}; octant <= 7; ++octant)
    {
        // Set range for octant
        if (((octant + 1) / 2) % 2) // := f tt ff tt f
        {
            range = octantWidth;
        }
        else
        {
            range = octantHeight;
        }
#if defined(DEBUG) && defined(DEBUG_SHADOW)
        snx::debug::cliPrint(
            "\nOctant: ",
            octant,
            "\n\n");
#endif

        calculateVisibilitiesInOctant(
            octant,
            map,
            heroPosition,
            range);
    }
#if defined(DEBUG) && defined(DEBUG_FOG)
    for (Fog const& fog : fogsToRender_.values())
    {
        snx::debug::cliLog(
            "Fog",
            fog.tilePosition(),
            ": ",
            (fog.isFogOpaque()) ? "invis" : "seen",
            "\n");
    }
#endif
}
