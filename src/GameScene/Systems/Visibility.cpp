#include "Visibility.h"

#include "Debugger.h"
#include "Fog.h"
#include "Shadow.h"
#include "TileData.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>

void Visibility::addShadow(
    std::vector<Shadow>& shadowline,
    Vector2I const& sectorPosition)
{
    Shadow shadowNew{sectorPosition};

    float shadowNewLeftAtTop{shadowNew.getLeftAtTop(sectorPosition)};
    float shadowNewRightAtBottom{shadowNew.getRightAtBottom(sectorPosition)};

    int shadowContainingLeftEnd{-1};
    int shadowContainingRightEnd{-1};

    for (size_t i{0}; i < shadowline.size(); ++i)
    {
#ifdef DEBUG
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
        if (shadowNewRightAtBottom < shadowline[i].getLeftAtBottom(sectorPosition))
        {
            // New ends before current
            continue;
        }

        // _old__||
        //            |--new-
        if (shadowline[i].getRightAtTop(sectorPosition) < shadowNewLeftAtTop)
        {
            // Current ends before new
            continue;
        }

        // ||__old_   _old__||
        //  |--new-   -new--|
        if (
            shadowline[i].getLeftAtTop(sectorPosition) <= shadowNewLeftAtTop
            && shadowNewRightAtBottom <= shadowline[i].getRightAtBottom(sectorPosition))
        {
            // Old contains new
            shadowContainingLeftEnd = i;
            shadowContainingRightEnd = i;
            break;
        }

        //        ||__old_   _old__||
        // |--new-   -new--|
        if (
            shadowNewLeftAtTop < shadowline[i].getLeftAtTop(sectorPosition)
            && shadowline[i].getLeftAtBottom(sectorPosition) <= shadowNewRightAtBottom
            && shadowNewRightAtBottom <= shadowline[i].getRightAtBottom(sectorPosition))
        {
            // Merge shadows if another shadow contains left end already
            if (shadowContainingLeftEnd > -1)
            {
#ifdef DEBUG
                snx::debug::cliLog("Merge: Extend shadow[", i, "] to the right\n");
#endif
                // Adjust remaining
                shadowline[shadowContainingLeftEnd].setSlopeRight(shadowline[i].slopeRight());
#ifdef DEBUG
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
#ifdef DEBUG
            snx::debug::cliLog("Extend left\n");
#endif
            shadowline[i].setSlopeLeft(shadowNew.slopeLeft());
#ifdef DEBUG
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
            shadowline[i].getLeftAtTop(sectorPosition) <= shadowNewLeftAtTop
            && shadowNewLeftAtTop <= shadowline[i].getRightAtTop(sectorPosition)
            && shadowline[i].getRightAtBottom(sectorPosition) < shadowNewRightAtBottom)
        {
            // Merge shadows if another shadow contains right end already
            if (shadowContainingRightEnd > -1)
            {
                // Adjust remaining
#ifdef DEBUG
                snx::debug::cliLog("Merge: Extend shadow[", i, "] to the left\n");
#endif
                shadowline[shadowContainingRightEnd].setSlopeLeft(shadowline[i].slopeLeft());
#ifdef DEBUG
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
#ifdef DEBUG
            snx::debug::cliLog("Extend right\n");
#endif
            shadowline[i].setSlopeRight(shadowNew.slopeRight());
#ifdef DEBUG
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
#ifdef DEBUG
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
#ifdef DEBUG
            snx::debug::cliLog(
                "SectorPosition",
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

#ifdef DEBUG
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

                // isTileVisible(); //-> bool isVisible
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
#ifdef DEBUG
                        snx::debug::cliLog(
                            "Tile is (partly) visible",
                            "\n");
#endif
                        continue;
                    }

                    isVisible = false;
#ifdef DEBUG
                    snx::debug::cliLog(
                        "Tile is invisible",
                        "\n");
#endif
                    break;
                } // Shadowline

                // Update visibility
                // updateVisibility();
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
#ifdef DEBUG
            snx::debug::cliLog(
                "Tile is opaque, add shadow...",
                "\n");
#endif
            addShadow(shadowline, octantPosition);
        } // Octant column
    } // Octant row
}

void Visibility::update(
    Tiles& map,
    RectangleEx const& viewport,
    Vector2I const& heroPosition)
{
    // Input
    int sectorWidth{2 + static_cast<int>((viewport.width() / 2) / TileData::TILE_SIZE)};
    int sectorHeight{2 + static_cast<int>((viewport.height() / 2) / TileData::TILE_SIZE)};

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
            range = sectorWidth;
        }
        else
        {
            range = sectorHeight;
        }
#ifdef DEBUG
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
    for (Fog const& fog : fogsToRender_.values())
    {
        snx::debug::cliLog(
                "Fog", 
                fog.tilePosition(), 
                ": ", 
                (fog.isFogOpaque()) ? "invis" : "seen",
                "\n");
    }
}
