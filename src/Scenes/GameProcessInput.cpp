#include "Game.h"

#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"

void moveCursor(Vector2Int& cursorPosition, bool mouseActive);
void processEdgePan(const Vector2& cursorWorldPosition, const Vector2& referenceWorldPosition, bool mouseActive);
void processZoom();
void processSelection(Unit& unit, const Vector2Int& cursorPosition);

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

    moveCursor(cursor.position, mouseActive);

    processEdgePan(
        positionToWorld(cursor.position),
        positionToWorld(hero.position),
        mouseActive);

    processZoom();

    processSelection(
        hero,
        cursor.position);

    // Set unit movment target
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        for (auto& tile : world.framedMapOverlay().values())
        {
            if (!Vector2IntEquals(tile.position, cursor.position))
                continue;

            // Set movement target
        }
    }
}

void moveCursor(Vector2Int& cursorPosition, bool mouseActive)
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
        Vector2Int dir{};

        // Get direction
        switch (GetKeyPressed())
        {
        case KEY_W:
        case KEY_UP:
            if (positionToScreen(cursorPosition).y > 0)
                dir = V_UP;
            break;
        case KEY_A:
        case KEY_LEFT:
            if (positionToScreen(cursorPosition).x > 0)
                dir = V_LEFT;
            break;
        case KEY_S:
        case KEY_DOWN:
            if (positionToScreen(cursorPosition).y < GetRenderHeight())
                dir = V_DOWN;
            break;
        case KEY_D:
        case KEY_RIGHT:
            if (positionToScreen(cursorPosition).x < GetRenderWidth())
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
        Vector2Int newCursorPosition{cursorPosition.x + (dir.x * scale),
                                     cursorPosition.y + (dir.y * scale)};

        // If out of screen and boost applied
        if (scale > 1 && !CheckCollisionPointRec(positionToScreen(newCursorPosition), GetRenderRec()))
        {
            // Check again without boost
            scale = 1;

            newCursorPosition = {cursorPosition.x + (dir.x * scale),
                                 cursorPosition.y + (dir.y * scale)};

            // If still out of screen
            if (!CheckCollisionPointRec(positionToScreen(newCursorPosition), GetRenderRec()))
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
    auto screenCursor{GetWorldToScreen2D(cursorWorldPosition, dtb::Globals::camera())};
    auto screenReference{GetWorldToScreen2D(referenceWorldPosition, dtb::Globals::camera())};

    if (!CheckCollisionPointRec(screenCursor, dtb::Constants::cursorDeadzone()))
    {
        if (mouseActive && dt < 0.1f)
            return;

        dt = 0;

        // Adjust cursor position relative to deadzone
        if (screenCursor.x < dtb::Constants::cursorDeadzone().x &&
            screenReference.x < dtb::Constants::cursorDeadzone().x + dtb::Constants::cursorDeadzone().width)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_LEFT,
                    TILE_SIZE));
        }

        if (screenCursor.x > (dtb::Constants::cursorDeadzone().x + dtb::Constants::cursorDeadzone().width) &&
            screenReference.x > dtb::Constants::cursorDeadzone().x)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_RIGHT,
                    TILE_SIZE));
        }

        if (screenCursor.y < dtb::Constants::cursorDeadzone().y &&
            screenReference.y < dtb::Constants::cursorDeadzone().y + dtb::Constants::cursorDeadzone().height)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_UP,
                    TILE_SIZE));
        }

        if (screenCursor.y > (dtb::Constants::cursorDeadzone().y + dtb::Constants::cursorDeadzone().height) &&
            screenReference.y > dtb::Constants::cursorDeadzone().y)
        {
            dtb::Globals::moveCamera(
                Vector2IntScale(
                    V_DOWN,
                    TILE_SIZE));
        }
    }
}

void processZoom()
{
    Camera2D& camera{dtb::Globals::camera()};
    float wheel = GetMouseWheelMove();

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

void processSelection(Unit& unit, const Vector2Int& cursorPosition)
{
    // Select unit
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        if (unit.position == cursorPosition)
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