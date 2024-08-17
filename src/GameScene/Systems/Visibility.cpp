#include "Visibility.h"

#include "TileData.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>

void Visibility::importSolidRectangles(Tiles& map, RectangleEx const& mapPanel, Camera2D const& camera)
{
    std::unordered_set<Vector2I> const& visionBlockingTilePositions{map.blocksVisions()};

    // solidRectangles_.reserve(visionBlockingTilePositions.size());
    for (Vector2I const& tilePosition : visionBlockingTilePositions)
    {
        Vector2 const renderPosition{UnitConversion::tileToScreen(tilePosition, camera)};

        if (!CheckCollisionPointRec(renderPosition, mapPanel))
        {
            continue;
        }

        solidRectangles_.push_back(
            RectangleEx{
                Vector2{Vector2SubtractValue(
                    renderPosition,
                    (TileData::TILE_SIZE_HALF))},
                Vector2{Vector2AddValue(
                    renderPosition,
                    (TileData::TILE_SIZE_HALF))}});
    }
}

void Visibility::setTargetsToCheck(RectangleEx const& mapPanel, Camera2D const& camera)
{
    Vector2I const firstTilePosition{
        UnitConversion::screenToTile(
            mapPanel.topLeft(),
            camera)};

    Vector2 const firstCorner{Vector2SubtractValue(
        UnitConversion::tileToWorld(
            firstTilePosition),
        (TileData::TILE_SIZE_HALF))};

    // Add first tile
    tilesToCheck_.emplace(firstTilePosition, false);

    // Add first corner
    cornersToCheck_.push_back(firstCorner);

    // Set tile count
    Vector2I const tileCount{
        static_cast<int>((mapPanel.width() / TileData::TILE_SIZE)) + 2,
        static_cast<int>(mapPanel.height() / TileData::TILE_SIZE) + 2};

    // Set corner count
    Vector2I const cornerCount{
        static_cast<int>((mapPanel.width() / TileData::TILE_SIZE)) + 3,
        static_cast<int>(mapPanel.height() / TileData::TILE_SIZE) + 3};

    // Add remaining tilePositions
    for (int x{0}; x < tileCount.x; ++x)
    {
        for (int y{0}; y < tileCount.y; ++y)
        {
            tilesToCheck_.emplace(
                Vector2Add(
                    firstTilePosition,
                    Vector2I{
                        x,
                        y}),
                false);
        }
    }

    // Add remaining corners
    for (int x{0}; x < cornerCount.x; ++x)
    {
        for (int y{0}; y < cornerCount.y; ++y)
        {
            cornersToCheck_.push_back(
                Vector2{
                    Vector2Add(
                        firstCorner,
                        Vector2Scale(
                            Vector2{
                                static_cast<float>(x),
                                static_cast<float>(y)},
                            TileData::TILE_SIZE))});
        }
    }
}

void Visibility::importMapData(Tiles& map, RectangleEx const& mapPanel, Camera2D const& camera)
{
    importSolidRectangles(map, mapPanel, camera);
    setTargetsToCheck(mapPanel, camera);
}

void Visibility::checkCorners(Vector2I const& heroPosition, Camera2D const& camera)
{
    size_t cornerCount{cornersToCheck_.size()};

    for (size_t i{0}; i < cornerCount; ++i)
    {
        Vector2 rayStart{UnitConversion::tileToScreen(heroPosition, camera)};

        for (RectangleEx const& solidRectangle : solidRectangles_)
        {
            // If not obsturcted, corner remains in vector
            if (CheckCollisionLineRec(
                    rayStart,
                    cornersToCheck_[i],
                    solidRectangle))
            {
                cornersToCheck_.erase(cornersToCheck_.begin() + i);
                --cornerCount;
                --i;
                continue;
            }

            // #ifdef DEBUG
            //             BeginDrawing();
            //             DrawLineV(rayStart, cornersToCheck_[i], RED);
            //             DrawRectangleRec(Rectangle{cornersToCheck_[i].x, cornersToCheck_[i].y, TileData::TILE_SIZE, TileData::TILE_SIZE}, GREEN);
            //             EndDrawing();
            // #endif

            ++i;
        }
    }
}

void Visibility::calculateVisiblity()
{
    for (Vector2 const& corner : cornersToCheck_)
    {
        for (Vector2 const offset : {
                 Vector2{-TileData::TILE_SIZE_HALF, -TileData::TILE_SIZE_HALF},
                 Vector2{-TileData::TILE_SIZE_HALF, TileData::TILE_SIZE_HALF},
                 Vector2{TileData::TILE_SIZE_HALF, -TileData::TILE_SIZE_HALF},
                 Vector2{TileData::TILE_SIZE_HALF, TileData::TILE_SIZE_HALF},
             })
        {
            Vector2I tilePosition{
                UnitConversion::worldToTile(
                    Vector2Add(
                        corner,
                        offset))};

            // Validate position exists
            if (tilesToCheck_.find(tilePosition) == tilesToCheck_.end())
            {
                continue;
            }

            // For having convex tile visible, trigger at >0 (first visible corner makes tile visible)
            // else trigger at >1 (needs value check)

            // Value check
            bool& hasVisibleCorner{tilesToCheck_.at(tilePosition)};

            if (!hasVisibleCorner)
            {
                hasVisibleCorner = true;
                continue;
            }

            // Add position as visible
            visibilities_.visibile.push_back(tilePosition);
            tilesToCheck_.erase(tilePosition);
        }
    }

    for (auto const& v : tilesToCheck_)
    {
        visibilities_.invisible.push_back(v.first);
    }
}

void Visibility::exportVisiblity(Tiles& map)
{
    // Update invisible tiles
    for (Vector2I const& position : visibilities_.invisible)
    {
        if (map.visibilityID(position) != VisibilityID::visible)
        {
            continue;
        }

        map.setVisibilityID(position, VisibilityID::seen);
    }

    // Update visible tiles
    for (Vector2I const& position : visibilities_.visibile)
    {
        map.setVisibilityID(
            position,
            VisibilityID::visible);
    }
}

void Visibility::reset()
{
    visibilities_.invisible.clear();
    visibilities_.visibile.clear();
    cornersToCheck_.clear();
    tilesToCheck_.clear();
    solidRectangles_.clear();
}

void Visibility::update(
    Tiles& map,
    RectangleEx const& mapPanel,
    Camera2D const& camera,
    Vector2I const& heroPosition)
{
    importMapData(map, mapPanel, camera);
    checkCorners(heroPosition, camera);
    calculateVisiblity();
    exportVisiblity(map);
    reset();
}
