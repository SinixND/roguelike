#include "App.h"

// #define DEBUG_TILEINFO

#include "AppConfigs.h"
#include "AppData.h"
#include "DeveloperMode.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Game.h"
#include "GameFont.h"
#include "InputHandler.h"
#include "InputId.h"
#include "VisibilitySystem.h"
#include <raygui.h>
#include <raylib.h>

#if defined( EMSCRIPTEN )
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
#include "Objects.h"
#include <string>
#endif

void setupRaylib( AppConfig const& config )
{
    //* Raylib flags
    SetConfigFlags( FLAG_WINDOW_RESIZABLE );
    if ( config.vSync )
    {
        SetConfigFlags( FLAG_VSYNC_HINT );
    }

    //* Initialize window
    InitWindow(
        config.windowWidth,
        config.windowHeight,
        "Roguelike"
    );

    //* Raylib Settings
    SetWindowIcon( AppData::FAVICON );
    SetWindowMinSize(
        AppData::WINDOE_WIDTH_MIN,
        AppData::WINDOE_HEIGHT_MIN
    );

#if defined( EMSCRIPTEN )
    MaximizeWindow();
#endif

    SetExitKey( KEY_F4 );
    SetTargetFPS( AppData::FPS_TARGET );
    SetTextureFilter(
        GetFontDefault().texture,
        TEXTURE_FILTER_POINT
    );

    //* Fonts
    GameFont::load();

    GuiSetStyle(
        DEFAULT,
        TEXT_SIZE,
        GameFont::fontSize
    );
}

void setupNcurses() {}

void setupFrameworks( AppConfig const& config )
{
    setupRaylib( config );
    setupNcurses();
}

void updateWindowState()
{
    if ( IsKeyPressed( KEY_F11 ) )
    {
        if ( IsWindowMaximized() )
        {
            RestoreWindow();
        }

        else
        {
            MaximizeWindow();
        }
    }

    if ( IsWindowResized() )
    {
        snx::EventDispatcher::notify( EventId::WINDOW_RESIZED );
    }
}

void updateDeveloperMode()
{
    if ( IsKeyPressed( KEY_F1 ) )
    {
        DeveloperMode::toggle();
    }
}

InputId getUserInput( InputHandler& inputHandler )
{
    InputId inputId{};

    //* Take input from mouse, keys or gestures
    inputId = inputHandler.fromKeyboard();

    if ( inputId != InputId::NONE )
    {
        return inputId;
    }

    inputId = inputHandler.fromMouse();

    if ( inputId != InputId::NONE )
    {
        return inputId;
    }

    inputId = inputHandler.fromGesture();

    return inputId;
}

/// @brief C-Style void argument in function signature needed for emscripten
void updateApp( void* arg )
{
    App& app = *(App*)arg;

    updateWindowState();
#if defined( DEBUG )
    updateDeveloperMode();
#endif
    app.inputHandler.currentInputId = getUserInput( app.inputHandler );

    if ( app.inputHandler.currentInputId == InputId::TOGGLE_CURSOR )
    {
        app.inputHandler.toggleCursorState();
        snx::EventDispatcher::notify( EventId::CURSOR_TOGGLE );
    }

    app.dt = GetFrameTime();

    app.cursor = CursorModule::update(
        app.cursor,
        app.scenes.game.gameCamera.camera,
        Convert::worldToTile( app.game.hero.position )
    );

    switch ( app.stateId )
    {
        default:
        case App::StateId::RUN_GAME:
        {
            app.game = GameModule::update(
                app.game,
                app.scenes.game.gameCamera,
                app.cursor,
                app.inputHandler.currentInputId,
                app.dt
            );

            app.scenes.game = SceneGameModule::update(
                app.scenes.game,
                app.game.hero,
                app.game.world,
                app.cursor,
                app.game.state,
                app.inputHandler.currentInputId
            );

            break;
        }

        case App::StateId::GAME_OVER:
        {
            SceneGameOverModule::update( app.scenes.gameOver );

            break;
        }
    }
}

void App::setupAppEvents()
{
#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
    snx::EventDispatcher::addListener(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{ Convert::worldToTile( app.cursor.position ) };

            Tiles& tiles{ app.game.world.currentMap->tiles };

            if ( !tiles.ids.contains( cursorPos ) )
            {
                return;
            }

            size_t tileId{ tiles.ids.at( cursorPos ) };

            snx::Debugger::cliPrint( "\n" );
            snx::Debugger::cliLog( "TILE:\n" );

            snx::Debugger::cliPrint(
                "TilePosition: "
                + std::to_string( cursorPos.x )
                + ", "
                + std::to_string( cursorPos.y )
                + "\n"
            );

            snx::Debugger::cliPrint(
                "WorldPixel: "
                + std::to_string( tiles.positions.at( tileId ).x )
                + ", "
                + std::to_string( tiles.positions.at( tileId ).y )
                + "\n"
            );

            snx::Debugger::cliPrint(
                "RenderId: "
                + std::to_string( static_cast<int>( tiles.renderIds.at( tileId ) ) )
                + "\n"
            );

            snx::Debugger::cliPrint(
                "VisibilityId: "
                + std::to_string( static_cast<int>( tiles.visibilityIds.at( tileId ) ) )
                + "\n"
            );

            snx::Debugger::cliPrint(
                "IsSolid: "
                + std::to_string( tiles.isSolids.contains( tileId ) )
                + "\n"
            );

            snx::Debugger::cliPrint(
                "IsOpaque: "
                + std::to_string( tiles.isOpaques.contains( tileId ) )
                + "\n"
            );

            Objects& objects{ app.game.world.currentMap->objects };

            if ( objects.ids.contains( Convert::worldToTile( app.cursor.position ) ) )
            {
                size_t objectIdx{ objects.ids.index( Convert::worldToTile( app.cursor.position ) ) };

                snx::Debugger::cliLog( "OBJECT\n" );

                snx::Debugger::cliPrint(
                    "Name: "
                    + app.game.world.currentMap->objects.names[objectIdx]
                    + "\n"
                );

                snx::Debugger::cliPrint(
                    "Actions: "
                    + app.game.world.currentMap->objects.actions[objectIdx]
                    + "\n"
                );

                snx::Debugger::cliPrint(
                    "RenderId: "
                    + std::to_string( static_cast<int>( app.game.world.currentMap->objects.renderIds[objectIdx] ) )
                    + "\n"
                );

                snx::Debugger::cliPrint(
                    "Event: "
                    + std::to_string( static_cast<int>( app.game.world.currentMap->objects.eventIds[objectIdx] ) )
                    + "\n"
                );
            }

            if ( app.game.world.currentMap->enemies.ids.contains( cursorPos ) )
            {
                snx::Debugger::cliLog( "ENEMY\n" );

                snx::Debugger::cliPrint(
                    "Id: "
                    + std::to_string( app.game.world.currentMap->enemies.ids.at( cursorPos ) )
                    + "\n"
                );
            }
        }
    );
#endif
    snx::EventDispatcher::addListener(
        EventId::CURSOR_TOGGLE,
        [&]()
        {
            cursor = CursorModule::toggle( cursor );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::WINDOW_RESIZED,
        [&]()
        {
            scenes.game.panels = GamePanelsModule::init( scenes.game.panels );
            scenes.gameOver = SceneGameOverModule::init( scenes.gameOver );
            scenes.game.overlays = OverlaysModule::init( scenes.game.overlays );

            scenes.game.gameCamera = GameCameraModule::init(
                scenes.game.gameCamera,
                scenes.game.panels.map.box(),
                game.hero.position
            );

            game.world.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                game.world.currentMap->tiles,
                game.world.currentMap->fogs,
                GameCameraModule::viewportInTiles( scenes.game.gameCamera ),
                Convert::worldToTile( game.hero.position ),
                game.hero.visionRange
            );
        },
        true
    );

    snx::EventDispatcher::addListener(
        EventId::GAME_OVER,
        [&]()
        {
            stateId = App::StateId::GAME_OVER;
        }
    );
}

void App::init(
    AppConfig const& config
)
{
    setupFrameworks( config );

    game = GameModule::init( game );

    scenes.game = SceneGameModule::init(
        scenes.game,
        game.hero,
        game.world
    );

    scenes.gameOver = SceneGameOverModule::init( scenes.gameOver );

    setupAppEvents();
}

void App::run()
{
#if defined( EMSCRIPTEN )
    emscripten_set_main_loop_arg(
        updateApp,
        &appIO,
        60 /*FPS*/,
        1 /*Simulate infinite loop*/
    );
#else
    while ( !( WindowShouldClose() ) )
    {
        updateApp( this );
    }
#endif
}

void App::deinit()
{
    GameFont::unload();
    SceneGameModule::deinitialize( scenes.game );

    //* Close window and opengl context
    CloseWindow();
}
