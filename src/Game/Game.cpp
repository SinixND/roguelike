#include "Game.h"

#include "Constants.h"
#include "Entity.h"
#include "Graphic.h"
#include "LayerID.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
#include "Render.h"
#include "RenderID.h"
#include "TileMap.h"
#include "TileTransformation.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "World.h"
#include "raylibEx.h"
#include <bitset>
#include <chrono>
#include <cmath>
#include <iostream>
#include <math.h>
#include <memory>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <thread>
#include <vector>

namespace
{
    // Variables needed for multiple stepps in main loop
    World gameWorld{};

    Entity cursor{
        Position(),
        Graphic(
            RenderID::cursor,
            LayerID::UI)};

    Unit hero{
        Position(),
        Graphic(
            RenderID::hero,
            LayerID::object),
        Movement(5, 50),
        VisibilityID::visible,
        5};

    bool isInputBlocked{false};
    bool isPathShown{false};
}

void GameScene::initialize()
{
}

namespace 
{
    void moveCursor(Position& cursorPosition, bool mouseActive);
    void processEdgePan(const Vector2& cursorWorldPosition, bool mouseActive);
    void processZoom();
    void processSelection(Unit& unit, const Vector2i& cursorPosition);
}

void GameScene::processInput()
{
    if (isInputBlocked) return;

    // Toggle debug mode
    if (IsKeyDown(KEY_F1))
        dtb::setDebugMode(!dtb::debugMode());

    // Toggle between mouse or key control for cursor
    static bool mouseActive{true};

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        mouseActive = !mouseActive;

        if (mouseActive)
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }

    moveCursor(cursor.position, mouseActive);

    processEdgePan(
        TileTransformation::positionToWorld(cursor.position.tilePosition()),
        mouseActive);

    processZoom();

    processSelection(
        hero,
        cursor.position.tilePosition());

    // Set unit movment target
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        // Check if target is valid
        if (
            gameWorld.mapOverlay().contains(cursor.position.tilePosition()) &&
            !Vector2Equals(cursor.position.tilePosition(), hero.position.tilePosition()))
        {
            // Set movement target
            hero.movement.setTarget(cursor.position.tilePosition());
            hero.movement.setIsTargetSet(true);
        }
    }
}

namespace 
{
    void showUnitRange(bool& moveRangeShown, Unit& unit, World& world);
    Path& showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown);
    void triggerMovement(Unit& unit, Path& path, bool& inputBlocked);
    void processMovement(Unit& unit, bool& inputBlocked);
    void processVision(Unit& unit, TileMap& tiles);
}

void GameScene::updateState()
{
    static bool moveRangeShown{false};
    static Path path{};

    int condition = // A=isSelected, B=rangeShown
        (hero.isSelected ? (true << 1) : false) +
        (moveRangeShown ? (true << 0) : false);

    switch (condition)
    {
    default:
        break;
    case 0:
        // 0 0 // not selected, not shown -> Do nothing

        break;

    case 1:
    { // 0 1 // not selected, range shown -> Hide range
        gameWorld.mapOverlay().clear();
        moveRangeShown = false;
        isPathShown = false;
    }
    break;

    case 2:
    { // 1 0 // selected, range not shown -> Show range
        showUnitRange(
            moveRangeShown,
            hero,
            gameWorld);
    }
    break;

    case 3:
    { // 1 1 // selected, range shown -> Show path
        path = showPath(
            hero.position.tilePosition(),
            cursor.position.tilePosition(),
            hero.movement.range,
            gameWorld,
            isPathShown);
    }
    break;
    }

    triggerMovement(hero, path, isInputBlocked);

    // Unblock input if target is reached
    processMovement(hero, isInputBlocked);

    processVision(hero, gameWorld.currentMap());
}

void GameScene::renderOutput()
{
    BeginMode2D(dtb::camera());

    // Layer map
    for (auto& tile : gameWorld.currentMap().values())
    {
        render(tile.position.get(), tile.graphic, tile.visibilityID);
    }

    // Layer map overlay
    for (auto& tile : gameWorld.mapOverlay().values())
    {
        render(tile.position.get(), tile.graphic, tile.visibilityID);
    }

    // Layer framed map overlay
    for (auto& tile : gameWorld.framedMapOverlay().values())
    {
        render(tile.position.get(), tile.graphic, tile.visibilityID);
    }

    // Render object layer
    render(hero.position.get(), hero.graphic);

    // Render UI layer
    render(cursor.position.get(), cursor.graphic);

    EndMode2D();

    // Draw text for current level
    //=================================
    const char* currentLevel{TextFormat("Level %i", gameWorld.currentLevel())};

    Font& font{dtb::font()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    Vector2 textDimensions{MeasureTextEx(font, currentLevel, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((GetRenderWidth() / 2.0f) - (textDimensions.x / 2)),
            10},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
    //=================================
}

void GameScene::postOutput()
{
    // Clear path
    gameWorld.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}

namespace
{
    void moveCursor(Position& cursorPosition, bool mouseActive)
    {
        // Cursor control
        if (mouseActive)
        {
            // Mouse controlled cursor
            cursorPosition.setTilePosition(TileTransformation::getMouseTile());
        }
        else
        {
            // Key controlled cursor
            Vector2i dir{};

            // Get direction
            switch (GetKeyPressed())
            {
            case KEY_W:
            case KEY_UP:
            {
                if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).y > 0)
                    dir = V_UP;
            }
            break;
            case KEY_A:
            case KEY_LEFT:
            {
                if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).x > 0)
                    dir = V_LEFT;
            }
            break;
            case KEY_S:
            case KEY_DOWN:
            {
                if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).y < GetRenderHeight())
                    dir = V_DOWN;
            }
            break;
            case KEY_D:
            case KEY_RIGHT:
            {
                if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).x < GetRenderWidth())
                    dir = V_RIGHT;
            }
            break;

            default:
                break;
            }

            // Boost when shift is pressed
            int scale{1};
            if (IsKeyDown(KEY_LEFT_SHIFT))
                scale = 4;

            // Check if cursor would go out of screen
            Vector2i newCursorPosition{cursorPosition.tilePosition().x + (dir.x * scale),
                                    cursorPosition.tilePosition().y + (dir.y * scale)};

            // If out of screen and boost applied
            if (scale > 1 && !CheckCollisionPointRec(TileTransformation::positionToScreen(newCursorPosition), GetRenderRec()))
            {
                // Check again without boost
                scale = 1;

                newCursorPosition = {cursorPosition.tilePosition().x + (dir.x * scale),
                                    cursorPosition.tilePosition().y + (dir.y * scale)};

                // If still out of screen
                if (!CheckCollisionPointRec(TileTransformation::positionToScreen(newCursorPosition), GetRenderRec()))
                    return;
            }

            cursorPosition.setTilePosition(newCursorPosition);
        }
    }

    void processEdgePan(const Vector2& cursorWorldPosition, bool mouseActive)
    {
        static float dt{};
        static const float PAN_SPEED{0.1f}; // time in seconds to next pan progress
        dt += GetFrameTime();

        // Check if cursor is outside of edge pan area
        auto screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::camera())};

        Rectangle edgePanArea{GetRectangle(
            Vector2AddValue(
                Vector2{0, 0},
                EDGE_PAN_FRAME_WEIGHT),
            Vector2SubtractValue(
                Vector2{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())},
                EDGE_PAN_FRAME_WEIGHT))};

        if (!CheckCollisionPointRec(screenCursor, edgePanArea))
        {
            if (mouseActive && dt < PAN_SPEED)
                return;

            dt = 0;

            // Adjust cursor position relative to edge pan area while avoiding to pan too far
            Vector2i panDirection{V_NODIR};

            // Determine pan direction
            if (screenCursor.x < edgePanArea.x)
            {
                panDirection = Vector2Add(panDirection, V_LEFT);
            }
            else if (screenCursor.x > (edgePanArea.x + edgePanArea.width))
            {
                panDirection = Vector2Add(panDirection, V_RIGHT);
            }

            if (screenCursor.y < edgePanArea.y)
            {
                panDirection = Vector2Add(panDirection, V_UP);
            }
            else if (screenCursor.y > (edgePanArea.y + edgePanArea.height))
            {
                panDirection = Vector2Add(panDirection, V_DOWN);
            }

            // Check if pan is valid
            if (!CheckCollisionPointArea(
                    Vector2Add(
                        TileTransformation::screenToPosition(
                            {GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f}),
                        panDirection),
                    dtb::mapSize()))
                return;

            // Update camera
            dtb::moveCamera(Vector2Scale(panDirection, TILE_SIZE));
        }
    }

    void processZoom()
    {
        float wheel = GetMouseWheelMove();
        Camera2D& camera{dtb::camera()};

        if (wheel != 0)
        {
            camera.zoom += (wheel * ZOOM_STEP);
            if (camera.zoom < ZOOM_STEP) camera.zoom = ZOOM_STEP;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && (IsKeyPressed(KEY_KP_0) || IsKeyPressed(KEY_ZERO)))
        {
            camera.zoom = 1.0f;
        }
    }

    void processSelection(Unit& unit, const Vector2i& cursorPosition)
    {
        // Select unit
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
        {
            if (unit.position.tilePosition() == cursorPosition)
            {
                // Toggle isSelected
                unit.isSelected = !unit.isSelected;
            }
        }

        // Deselect unit
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
        {
            unit.isSelected = false;
        }
    }
    
    void showUnitRange(bool& moveRangeShown, Unit& unit, World& world)
    {
        // Show range of selected unit
        if (unit.isSelected && !moveRangeShown)
        {
            moveRangeShown = true;

            // Filter relevant tiles
            for (auto& steppedPositions : TileMapFilters::filterReachable(
                    world.currentMap(),
                    unit.movement.range,
                    unit.position.tilePosition()))
            {
                for (auto& steppedPosition : steppedPositions)
                {
                    // Add reachable tile to overlay
                    world.mapOverlay().createOrUpdate(
                        steppedPosition.tilePosition,
                        Tile(
                            steppedPosition.tilePosition,
                            Graphic(
                                RenderID::reachable,
                                LayerID::mapOverlay)));
                }
            }
        }
    }

    Path& showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown)
    {
        static Vector2i origin{};
        static Vector2i target{};
        static size_t range{};
        static Path path{};

        // Check if path input changed
        if (!(
                origin == unitPosition &&
                target == cursorPosition &&
                range == unitRange))
        {
            // Update input and path
            origin = unitPosition;
            target = cursorPosition;
            range = unitRange;

            path = findPath(
                world.mapOverlay(),
                unitPosition,
                cursorPosition,
                unitRange);

            if (!path.empty()) pathShown = true;
        }

        for (auto& steppedPosition : path)
        {
            world.framedMapOverlay().createOrUpdate(
                steppedPosition.tilePosition,
                Tile(
                    steppedPosition.tilePosition,
                    Graphic(
                        RenderID::path,
                        LayerID::mapOverlay)));
        }

        return path;
    }

    void triggerMovement(Unit& unit, Path& path, bool& inputBlocked)
    {
        if (unit.movement.isTargetSet() && !unit.movement.isMoving())
        {
            // Reset (doesnt interrupt movement)
            unit.movement.setIsTargetSet(false);

            // Setting path triggers movment
            unit.movement.setPath(path);

            inputBlocked = true;
        }
    }

    void processMovement(Unit& unit, bool& inputBlocked)
    {
        bool targetReached{false};
        if (unit.movement.isMoving())
        {
            // Move unit
            targetReached = unit.movement.move(
                unit.position.get(),
                GetFrameTime());
        }

        if (targetReached)
        {
            hero.isSelected = false;
            inputBlocked = false;
        }
    }

    void processVision(Unit& unit, TileMap& tileMap)
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

                //* if (dtb::debugMode())
                //* {
                //* BeginDrawing();
                //* BeginMode2D(dtb::camera());

                //* DrawCircleLinesV(tile->position.get(), 5, RED);

                //* EndMode2D();
                //* EndDrawing();
                //* }
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
                static_cast<float>(sqrt(pow(unitRelative, 2) * (1 + pow((directionTransformed.y / directionTransformed.x), 2)))),
                static_cast<float>(sqrt(pow(unitRelative, 2) * (1 + pow((directionTransformed.x / directionTransformed.y), 2))))};

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

                //* if (dtb::debugMode())
                //* {
                //* BeginDrawing();
                //* //* ClearBackground(BLACK);
                //* BeginMode2D(dtb::camera());

                //* DrawLineEx(rayStart, rayEnd, 1, GREEN);
                //* DrawCircleLinesV(tileHit->position.get(), 3, WHITE);

                //* EndMode2D();
                //* EndDrawing();
                //* }

                // End ray cast if opaque is hit
                if (tileHit->blocksVision)
                    break;
            }
        };

        //* if (dtb::debugMode())
        //* {
        //* std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //* }
    }
}
