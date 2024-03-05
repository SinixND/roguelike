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
#include "TilePositionConversion.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "World.h"
#include "raylibEx.h"
#include <bitset>
#include <iostream>
#include <memory>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

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

void moveCursor(Position& cursorPosition, bool mouseActive);
void processEdgePan(const Vector2& cursorWorldPosition, bool mouseActive);
void processZoom();
void processSelection(Unit& unit, const Vector2i& cursorPosition);
void processDeselection(Unit& unit);

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
        tilePositionToWorld(cursor.position.tilePosition()),
        mouseActive);

    processZoom();

    processSelection(
        hero,
        cursor.position.tilePosition());

    processDeselection(
        hero);

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

void moveCursor(Position& cursorPosition, bool mouseActive)
{
    // Cursor control
    if (mouseActive)
    {
        // Mouse controlled cursor
        cursorPosition.setTilePosition(getMouseTile());
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
            if (tilePositionToScreen(cursorPosition.tilePosition()).y > 0)
                dir = V_UP;
        }
        break;
        case KEY_A:
        case KEY_LEFT:
        {
            if (tilePositionToScreen(cursorPosition.tilePosition()).x > 0)
                dir = V_LEFT;
        }
        break;
        case KEY_S:
        case KEY_DOWN:
        {
            if (tilePositionToScreen(cursorPosition.tilePosition()).y < GetRenderHeight())
                dir = V_DOWN;
        }
        break;
        case KEY_D:
        case KEY_RIGHT:
        {
            if (tilePositionToScreen(cursorPosition.tilePosition()).x < GetRenderWidth())
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
        if (scale > 1 && !CheckCollisionPointRec(tilePositionToScreen(newCursorPosition), GetRenderRec()))
        {
            // Check again without boost
            scale = 1;

            newCursorPosition = {cursorPosition.tilePosition().x + (dir.x * scale),
                                 cursorPosition.tilePosition().y + (dir.y * scale)};

            // If still out of screen
            if (!CheckCollisionPointRec(tilePositionToScreen(newCursorPosition), GetRenderRec()))
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
                    screenToTilePosition(
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
}

void processDeselection(Unit& unit)
{
    // Deselect unit
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
    {
        unit.isSelected = false;
    }
}

void showUnitRange(bool& moveRangeShown, Unit& unit, World& world);
Path& showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown);
void triggerMovement(Unit& unit, Path& path, bool& inputBlocked);
void processMovement(Unit& unit, bool& inputBlocked);
void processVision(Unit& unit, TileMap& tiles);

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

void showUnitRange(bool& moveRangeShown, Unit& unit, World& world)
{
    // Show range of selected unit
    if (unit.isSelected && !moveRangeShown)
    {
        moveRangeShown = true;

        // Filter relevant tiles
        for (auto& steppedPositions : filterReachable(
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

    std::vector<Vector2i> tilesInVisionRange{
        filterInRange(
            tileMap,
            unit.visionRange + 2,
            unit.position.tilePosition())};

    // Reset "visible" tiles to "seen" in extended vision range before updating visibility
    for (auto& tilePosition : tilesInVisionRange)
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

    // Iterate outer positions of vision range (map independent)
    int x{unit.visionRange};
    int y{0};
    int dx{-1};
    int dy{1};

    do
    {
        // Set ray properties
        Vector2 direction{Vector2Subtract(
            tilePositionToWorld(Vector2i{x, y}),
            unit.position.get())};

        Ray2D ray{
            unit.position.get(),
            Vector2Normalize(
                direction)};

        // Increment ray until visionRange
        float maxLength{Vector2Length(direction)};
        for (float length{TILE_SIZE}; length < maxLength; length += (TILE_SIZE / 4))
        {
            Vector2 target{
                Vector2Add(
                    ray.position,
                    Vector2Scale(
                        ray.direction,
                        length))};

            Tile* targetTile{tileMap.at(worldToTilePosition(target))};

            targetTile->visibilityID = VisibilityID::visible;

            if (dtb::debugMode())
            {
                BeginDrawing();
                //* ClearBackground(BLACK);
                BeginMode2D(dtb::camera());

                DrawLineEx(ray.position, Vector2Scale(ray.direction, length), 1, GREEN);
                DrawCircleLinesV(targetTile->position.get(), 10, WHITE);

                EndMode2D();
                EndDrawing();
            }

            // End ray cast if solid is hit
            if (targetTile->isSolid)
                break;
        };

        // Update position
        if (x == unit.visionRange) dx = -1;
        if (x == -unit.visionRange) dx = 1;
        if (y == unit.visionRange) dy = -1;
        if (y == -unit.visionRange) dy = 1;

        x += dx;
        y += dy;
    } while (!(x == unit.visionRange && y == 0));
};

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