#include "VisibilitySystem.h"

// #define DEBUG_SHADOW

#include "Convert.h"
#include "DenseMap.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
#include <vector>

#if defined(DEBUG) && defined(DEBUG_SHADOW)
#include "TileData.h"
#endif

//* Shadow
Shadow::Shadow(Vector2I const& octantPosition)
{
    ShadowModule::setSlopeLeft(
        *this,
        octantPosition);
    ShadowModule::setSlopeRight(
        *this,
        octantPosition);
}

float ShadowModule::slopeLeft(
    Shadow const& shadow)
{
    return shadow.slopeLeft;
}

void ShadowModule::setSlopeLeft(
    Shadow& shadow,
    Vector2I const& octantPosition)
{
    shadow.slopeLeft = (octantPosition.y + 0.5f) / (octantPosition.x - 0.5f);
}

void ShadowModule::setSlopeLeft(
    Shadow& shadow,
    float slopeLeft)
{
    shadow.slopeLeft = slopeLeft;
}

float ShadowModule::slopeRight(
    Shadow const& shadow)
{
    return shadow.slopeRight;
}

void ShadowModule::setSlopeRight(
    Shadow& shadow,
    Vector2I const& octantPosition)
{
    shadow.slopeRight = std::max(EPSILON, (octantPosition.y - 0.5f) / (octantPosition.x + 0.5f));
}

void ShadowModule::setSlopeRight(
    Shadow& shadow,
    float slopeRight)
{
    shadow.slopeRight = slopeRight;
}

float ShadowModule::getLeftAtTop(
    Shadow const& shadow,
    Vector2I const& octantPosition)
{
    return (octantPosition.y + 0.5f) / shadow.slopeLeft;
}

float ShadowModule::getLeftAtBottom(
    Shadow const& shadow,
    Vector2I const& octantPosition)
{
    return (octantPosition.y - 0.5f) / shadow.slopeLeft;
}

float ShadowModule::getLeft(
    Shadow const& shadow,
    int octantPositionHeight)
{
    //* NOTE: x = y / m
    return (octantPositionHeight) / shadow.slopeLeft;
}

float ShadowModule::getRightAtTop(
    Shadow const& shadow,
    Vector2I const& octantPosition)
{
    return (octantPosition.y + 0.5f) / shadow.slopeRight;
}

float ShadowModule::getRightAtBottom(
    Shadow const& shadow,
    Vector2I const& octantPosition)
{
    return (octantPosition.y - 0.5f) / shadow.slopeRight;
}

float ShadowModule::getRight(
    Shadow const& shadow,
    int octantPositionHeight)
{
    //* NOTE: x = y / m
    return (octantPositionHeight) / shadow.slopeRight;
}

//* VisibilitySystem
#if defined(DEBUG) && defined(DEBUG_SHADOW)
void drawShadow(
    Shadow const& shadowNew,
    Vector2I const& octantPosition,
    int octant,
    Vector2I const& origin,
    float maxHeight)
{
    BeginDrawing();

    //* Draw left shadow projection
    DrawLineEx(
        Convert::octantToScreen(
            Vector2{
                shadowNew.getLeftAtTop(octantPosition) * TileData::TILE_SIZE,
                (octantPosition.y * TileData::TILE_SIZE) + TileData::TILE_SIZE_HALF},
            octant,
            origin,
            snx::debug::gcam().camera()),
        Convert::octantToScreen(
            Vector2{
                shadowNew.getLeft(maxHeight * TileData::TILE_SIZE),
                maxHeight * TileData::TILE_SIZE},
            octant,
            origin,
            snx::debug::gcam().camera()),
        1,
        YELLOW);

    //* Draw right shadow projection
    DrawLineEx(
        Convert::octantToScreen(
            Vector2{
                shadowNew.getRightAtBottom(octantPosition) * TileData::TILE_SIZE,
                (octantPosition.y * TileData::TILE_SIZE) - TileData::TILE_SIZE_HALF},
            octant,
            origin,
            snx::debug::gcam().camera()),
        Convert::octantToScreen(
            Vector2{
                shadowNew.getRight(maxHeight * TileData::TILE_SIZE),
                maxHeight * TileData::TILE_SIZE},
            octant,
            origin,
            snx::debug::gcam().camera()),
        1,
        YELLOW);

    EndDrawing();
}
#endif

void VisibilitySystem::updateShadowline(
    std::vector<Shadow>& shadowline,
    Vector2I const& octantPosition)
{
    Shadow shadowNew{octantPosition};

    float shadowNewLeftAtTop{ShadowModule::getLeftAtTop(
        shadowNew,
        octantPosition)};
    float shadowNewRightAtBottom{ShadowModule::getRightAtBottom(
        shadowNew,
        octantPosition)};

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

        //*           ||__old_
        //* -new--|
        if (shadowNewRightAtBottom < ShadowModule::getLeftAtBottom(shadowline[i], octantPosition))
        {
            //* New ends before current
            continue;
        }

        //* _old__||
        //*            |--new-
        if (ShadowModule::getRightAtTop(shadowline[i], octantPosition) < shadowNewLeftAtTop)
        {
            //* Current ends before new
            continue;
        }

        //* ||__old_   _old__||
        //*  |--new-   -new--|
        if (ShadowModule::getLeftAtTop(shadowline[i], octantPosition) <= shadowNewLeftAtTop
            && shadowNewRightAtBottom <= ShadowModule::getRightAtBottom(shadowline[i], octantPosition))
        {
            //* Old contains new
            shadowContainingLeftEnd = i;
            shadowContainingRightEnd = i;
            break;
        }

        //*        ||__old_   _old__||
        //* |--new-   -new--|
        if (shadowNewLeftAtTop < ShadowModule::getLeftAtTop(shadowline[i], octantPosition)
            && ShadowModule::getLeftAtBottom(shadowline[i], octantPosition) <= shadowNewRightAtBottom
            && shadowNewRightAtBottom <= ShadowModule::getRightAtBottom(shadowline[i], octantPosition))
        {
            //* Merge shadows if another shadow contains left end already
            if (shadowContainingLeftEnd > -1)
            {
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog("Merge: Extend shadow[", i, "] to the right\n");
#endif

                //* Adjust remaining
                ShadowModule::setSlopeRight(
                    shadowline[shadowContainingLeftEnd],
                    ShadowModule::slopeRight(shadowline[i]));

#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog(
                    "New shadow is (",
                    shadowline[shadowContainingLeftEnd].slopeLeft(),
                    ", ",
                    shadowline[shadowContainingLeftEnd].slopeRight(),
                    ")\n");
#endif

                //* Delete consumed
                shadowline.erase(shadowline.begin() + i);
                break;
            }

            //* Extend old to left
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog("Extend left\n");
#endif

            ShadowModule::setSlopeLeft(
                shadowline[i],
                ShadowModule::slopeLeft(shadowNew));

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

        //* ||__old_   _old__||
        //*          |--new-   -new--|
        if (ShadowModule::getLeftAtTop(shadowline[i], octantPosition) <= shadowNewLeftAtTop
            && shadowNewLeftAtTop <= ShadowModule::getRightAtTop(shadowline[i], octantPosition)
            && ShadowModule::getRightAtBottom(shadowline[i], octantPosition) < shadowNewRightAtBottom)
        {
            //* Merge shadows if another shadow contains right end already
            if (shadowContainingRightEnd > -1)
            {
                //* Adjust remaining
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog("Merge: Extend shadow[", i, "] to the left\n");
#endif

                ShadowModule::setSlopeLeft(shadowline[shadowContainingRightEnd], ShadowModule::slopeLeft(shadowline[i]));

#if defined(DEBUG) && defined(DEBUG_SHADOW)
                snx::debug::cliLog(
                    "New shadow is (",
                    shadowline[shadowContainingLeftEnd].slopeLeft(),
                    ", ",
                    shadowline[shadowContainingLeftEnd].slopeRight(),
                    ")\n");
#endif

                //* Delete consumed
                shadowline.erase(shadowline.begin() + i);
                break;
            }

            //* Extend old to right
#if defined(DEBUG) && defined(DEBUG_SHADOW)
            snx::debug::cliLog("Extend right\n");
#endif

            ShadowModule::setSlopeRight(
                shadowline[i],
                ShadowModule::slopeRight(shadowNew));

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
        //* Note: No case where new is covering old possible as shadows always get narrower the further from hero
    }

    //* If no intersection with new shadow
    if ((shadowContainingLeftEnd + shadowContainingRightEnd) < -1)
    {
        //* Add new shadow to shadow line
#if defined(DEBUG) && defined(DEBUG_SHADOW)
        snx::debug::cliLog("Create new shadow\n");
#endif

        shadowline.push_back(shadowNew);
    }
}

VisibilityId updatedVisibility(VisibilityId tileVisibilityOld)
{
    VisibilityId visibilityId{tileVisibilityOld};

    if (tileVisibilityOld == VisibilityId::VISIBILE)
    {
        //* Tile WAS visible
        visibilityId = VisibilityId::SEEN;
    }

    return visibilityId;
}

Fog updateFogOfWar(VisibilityId tileVisibilityOld)
{
    Fog fog{};

    if (tileVisibilityOld == VisibilityId::VISIBILE
        || tileVisibilityOld == VisibilityId::SEEN)
    {
        fog = Fog::TRANSPARENT;
    }

    else
    {
        fog = Fog::OPAQUE;
    }

    return fog;
}

void VisibilitySystem::calculateVisibilitiesInOctant(
    snx::DenseMap<Vector2I, Fog>& fogs,
    int octant,
    snx::DenseMap<Vector2I, VisibilityId>& visibilityIds,
    std::unordered_set<Vector2I> const& isOpaques,
    Vector2I const& heroPosition,
    int visionRange,
    int range)
{
    //* !!! Coordinates are usual cartesian !!!
    std::vector<Shadow> shadowline{};

#if defined(DEBUG) && defined(DEBUG_SHADOW)
    BeginDrawing();
#endif

    //* Iterate octant
    for (int octY{0}; octY < range; ++octY)
    {
        for (int octX{0}; octX <= octY + 1; ++octX)
        {
            Vector2I octantPosition{octX, octY};

            Vector2I tilePosition{
                Convert::octantToTile(
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

            if (!visibilityIds.contains(tilePosition))
            {
                continue;
            }

            VisibilityId tileVisibilityOld{visibilityIds.at(tilePosition)};

            //* < : Update only octant tiles including diagonal tiles (spare last row tile, needed for correct diagonal visibility)
            if (octX <= octY)
            {
                //* Skip test (. set invis) if shadowline already covers whole octant
                if (shadowline.size()
                    && ShadowModule::slopeLeft(shadowline[0]) < 0
                    && ShadowModule::slopeRight(shadowline[0]) < 1)
                {
#if defined(DEBUG) && defined(DEBUG_SHADOW)
                    snx::debug::cliLog(
                        "Shadow is max.",
                        "\n");
#endif

                    visibilityIds.at(tilePosition) = updatedVisibility(tileVisibilityOld);
                    fogs[tilePosition] = updateFogOfWar(tileVisibilityOld);

                    continue;
                } //* Max shadow

                //* Check visibility by intersection with shadow line elements
                bool isVisible{true};

                //* Check if tile is out of range (viewport and visionRange)
                if (!(
                        (octY < (range - 1))
                        && sqrt(pow(octX, 2) + pow(octY, 2)) < visionRange))
                {
                    isVisible = false;
                }
                else
                {
                    for (Shadow const& shadow : shadowline)
                    {
                        //* Check if visible:
                        //* If top-left tile corner is left (<) from slopeLeft (at same height = tileTop)
                        //* OR
                        //* If slopeRight is left (<) from bottom-right tile corner (at same height = tileBottom)
                        [[maybe_unused]] auto test{sqrt(pow(octX, 2) + pow(octY, 2))};
                        if (((octX - 0.5f) < (ShadowModule::getLeftAtTop(
                                 shadow,
                                 octantPosition))
                             || (ShadowModule::getRightAtBottom(
                                    shadow,
                                    octantPosition))
                                    < (octX + 0.5f)))
                        {
                            //* . visible (variable unchanged): top-left/bottom-right corner not in shadow

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
                    } //* Shadowline
                }

                //* Update visibility
                if (isVisible)
                {
                    //* Tile IS visible
                    visibilityIds.at(tilePosition) = VisibilityId::VISIBILE;
                }

                else
                {
                    visibilityIds.at(tilePosition) = updatedVisibility(tileVisibilityOld);
                    fogs[tilePosition] = updateFogOfWar(tileVisibilityOld);
                }
            } //* Octant tiles only

            //* Update shadow line
            if (!isOpaques.contains(tilePosition))
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
        } //* Octant column
    } //* Octant row

#if defined(DEBUG) && defined(DEBUG_SHADOW)
    EndDrawing();
#endif
}

void VisibilitySystem::update(
    snx::DenseMap<Vector2I, Fog>& fogs,
    snx::DenseMap<Vector2I, VisibilityId>& visibilityIds,
    std::unordered_set<Vector2I> const& isOpaques,
    RectangleExI const& viewportInTiles,
    int visionRange,
    Vector2I const& heroPosition)
{
    //* Input
    int quarterWidth{2 + (viewportInTiles.width() / 2)};
    int quarterHeight{2 + (viewportInTiles.height() / 2)};

    //* Init
    fogs.clear();

    visibilityIds.at(heroPosition) = VisibilityId::VISIBILE;

    //* Iterate octants
    //* Orientation dependent range (horizontal, vertical)
    int range{};

    for (int octant{0}; octant < 8; ++octant)
    {
        //* Set range for octant
        //* viewport dependent:
        if (((octant + 1) / 2) % 2) //* := f tt ff tt f
        {
            range = quarterWidth;
        }
        else
        {
            range = quarterHeight;
        }

#if defined(DEBUG) && defined(DEBUG_SHADOW)
        snx::debug::cliPrint(
            "\nOctant: ",
            octant,
            "\n\n");
#endif

        calculateVisibilitiesInOctant(
            fogs,
            octant,
            visibilityIds,
            isOpaques,
            heroPosition,
            visionRange,
            range);
    }
}
