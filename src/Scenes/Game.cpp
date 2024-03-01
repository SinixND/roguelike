#include "Game.h"

#include "LayerID.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Render.h"
#include "Renderable.h"
#include "TilePositionConversion.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <iostream>
#include <raygui.h>

namespace
{
    World gameWorld{};
    Cursor cursor{};
    Unit hero{};
}

void GameScene::initialize()
{
    cursor.graphic.renderID = RenderID::cursor;
    cursor.graphic.layerID = LayerID::UI;

    hero.graphic.renderID = RenderID::hero;
    hero.graphic.layerID = LayerID::object;
    hero.movement.range = 5;
    hero.movement.speed = 1;
    hero.movement.target = hero.position.tilePosition();
}

void moveCursor(Vector2i& cursorPosition, bool mouseActive);
void processEdgePan(const Vector2& cursorWorldPosition, const Vector2& referenceWorldPosition, bool mouseActive);
void processZoom();
void processSelection(Unit& unit, const Vector2i& cursorPosition);

void GameScene::processInput()
{
    // Toggle between mouse or key control for cursor
    static bool mouseActive{false};

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

    moveCursor(cursor.tilePosition, mouseActive);

    processEdgePan(
        tilePositionToWorld(cursor.tilePosition),
        tilePositionToWorld(hero.position.tilePosition()),
        mouseActive);

    processZoom();

    processSelection(
        hero,
        cursor.tilePosition);

    // Set unit movment target
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        // Set movement target
        hero.movement.target = cursor.tilePosition;
    }
}

void moveCursor(Vector2i& cursorPosition, bool mouseActive)
{
    // Cursor control
    if (mouseActive)
    {
        // Mouse controlled cursor
        setMouseTile(cursorPosition);
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
            if (tilePositionToScreen(cursorPosition).y > 0)
                dir = V_UP;
            break;
        case KEY_A:
        case KEY_LEFT:
            if (tilePositionToScreen(cursorPosition).x > 0)
                dir = V_LEFT;
            break;
        case KEY_S:
        case KEY_DOWN:
            if (tilePositionToScreen(cursorPosition).y < GetRenderHeight())
                dir = V_DOWN;
            break;
        case KEY_D:
        case KEY_RIGHT:
            if (tilePositionToScreen(cursorPosition).x < GetRenderWidth())
                dir = V_RIGHT;
            break;

        default:
            break;
        }

        // Boost when shift is pressed
        int scale{1};
        if (IsKeyDown(KEY_LEFT_SHIFT))
            scale = 4;

        // Check if cursor would go out of screen
        Vector2i newCursorPosition{cursorPosition.x + (dir.x * scale),
                                   cursorPosition.y + (dir.y * scale)};

        // If out of screen and boost applied
        if (scale > 1 && !CheckCollisionPointRec(tilePositionToScreen(newCursorPosition), GetRenderRec()))
        {
            // Check again without boost
            scale = 1;

            newCursorPosition = {cursorPosition.x + (dir.x * scale),
                                 cursorPosition.y + (dir.y * scale)};

            // If still out of screen
            if (!CheckCollisionPointRec(tilePositionToScreen(newCursorPosition), GetRenderRec()))
                return;
        }

        cursorPosition = newCursorPosition;
    }
}

void processEdgePan(const Vector2& cursorWorldPosition, const Vector2& referenceWorldPosition, bool mouseActive)
{
    static float dt{};
    dt += GetFrameTime();

    // Check if out of deadzone
    auto screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::camera())};
    auto screenReference{GetWorldToScreen2D(referenceWorldPosition, dtb::camera())};

    Rectangle cursorDeadzone{GetRectangle(
        Vector2AddValue(
            Vector2{0, 0},
            DEADZONE_PIXELS),
        Vector2SubtractValue(
            Vector2{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())},
            DEADZONE_PIXELS))};

    if (!CheckCollisionPointRec(screenCursor, cursorDeadzone))
    {
        if (mouseActive && dt < 0.1f)
            return;

        dt = 0;

        // Adjust cursor position relative to deadzone
        if (screenCursor.x < cursorDeadzone.x &&
            screenReference.x < cursorDeadzone.x + cursorDeadzone.width)
        {
            dtb::moveCamera(
                Vector2Scale(
                    V_LEFT,
                    TILE_SIZE));
        }

        if (screenCursor.x > (cursorDeadzone.x + cursorDeadzone.width) &&
            screenReference.x > cursorDeadzone.x)
        {
            dtb::moveCamera(
                Vector2Scale(
                    V_RIGHT,
                    TILE_SIZE));
        }

        if (screenCursor.y < cursorDeadzone.y &&
            screenReference.y < cursorDeadzone.y + cursorDeadzone.height)
        {
            dtb::moveCamera(
                Vector2Scale(
                    V_UP,
                    TILE_SIZE));
        }

        if (screenCursor.y > (cursorDeadzone.y + cursorDeadzone.height) &&
            screenReference.y > cursorDeadzone.y)
        {
            dtb::moveCamera(
                Vector2Scale(
                    V_DOWN,
                    TILE_SIZE));
        }
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

void showUnitRange(bool& moveRangeShown, Unit& unit, World& world);
void showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown);
void moveUnit(Unit& unit);

void GameScene::updateState()
{
    static bool moveRangeShown{false};
    static bool pathShown{false};

    showUnitRange(
        moveRangeShown,
        hero,
        gameWorld);

    // Clear overlay if no unit selected
    if (!hero.isSelected && moveRangeShown)
    {
        gameWorld.mapOverlay().clear();
        moveRangeShown = false;
        pathShown = false;
    }

    if (moveRangeShown)
    {
        showPath(
            hero.position.tilePosition(),
            cursor.tilePosition,
            hero.movement.range,
            gameWorld,
            pathShown);
    }

    moveUnit(hero);
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
                // Create reachable tile
                Tile reachableTile{};
                reachableTile.tilePosition = steppedPosition.tilePosition;
                reachableTile.graphic.renderID = RenderID::reachable;
                reachableTile.graphic.layerID = LayerID::mapOverlay;
                reachableTile.isSolid = false;

                // Add reachable tile to overlay
                world.mapOverlay().createOrUpdate(
                    steppedPosition.tilePosition,
                    reachableTile);
            }
        }
    }
}

void showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown)
{
    static Vector2i origin{};
    static Vector2i target{};
    static size_t range{};
    static std::vector<SteppedPosition> path{};

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
        Tile pathTile{};
        pathTile.tilePosition = steppedPosition.tilePosition;
        pathTile.graphic.renderID = RenderID::path;
        pathTile.graphic.layerID = LayerID::mapOverlay;
        pathTile.isSolid = false;

        world.framedMapOverlay().createOrUpdate(
            steppedPosition.tilePosition,
            pathTile);
    }
}

void moveUnit(Unit& unit)
{
    if (Vector2Equals(unit.movement.target, unit.position.tilePosition()))
        return;

    std::cout << "Move unit\n";
}

void GameScene::renderOutput()
{
    BeginMode2D(dtb::camera());

    // Layer map
    for (auto& tile : gameWorld.currentMap().values())
    {
        renderAtTile(tile.tilePosition, tile.graphic);
    }

    // Layer map overlay
    for (auto& tile : gameWorld.mapOverlay().values())
    {
        renderAtTile(tile.tilePosition, tile.graphic);
    }

    // Layer framed map overlay
    for (auto& tile : gameWorld.framedMapOverlay().values())
    {
        renderAtTile(tile.tilePosition, tile.graphic);
    }

    // Render object layer
    render(hero.position.get(), hero.graphic);

    // Render UI layer
    renderAtTile(cursor.tilePosition, cursor.graphic);

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

    if (dtb::debugMode())
    {
        DrawLineV(
            {GetRenderWidth() / 2.0f, 0},
            {GetRenderWidth() / 2.0f, static_cast<float>(GetRenderHeight())},
            RED);

        DrawLineV(
            {0, GetRenderHeight() / 2.0f},
            {static_cast<float>(GetRenderWidth()), GetRenderHeight() / 2.0f},
            RED);
    }
}

void GameScene::postOutput()
{
    // Clear path
    gameWorld.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}