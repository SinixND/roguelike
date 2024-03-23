#ifndef IG20231201203725
#define IG20231201203725
#include <raylib.h>

namespace snd
{
    class IScene
    {
    public:
        virtual void update(bool debugMode)
        {
            processInput();
            updateState();
            BeginDrawing();
            ClearBackground(BLACK);
            renderOutput();

            // Draw simple frame
            DrawRectangleLinesEx(
              Rectangle{
                0,
                0,
                static_cast<float>(GetRenderWidth()),
                static_cast<float>(GetRenderHeight())},
              1,
              GRAY);

            if (debugMode) {
                DrawFPS(0, 0);
            }

            EndDrawing();
            postOutput();
        };
        virtual void initialize() = 0;
        virtual void deinitialize() = 0;

        virtual ~IScene() = default;

    protected:
        virtual void processInput() = 0;
        virtual void updateState() = 0;
        virtual void renderOutput() = 0;
        virtual void postOutput() = 0;
    };

    //=====================================
}

#endif
