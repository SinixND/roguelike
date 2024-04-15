#ifndef IG20240117233028
#define IG20240117233028

#include "IScene.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <unordered_map>
#include <utility>

class dtb
{
    // Constants (only set once)
    //=================================
    // Font
private:
    static inline Font font_{};

public:
    static Font const& font() { return font_; }
    static void loadFont(char const* fileName) { font_ = LoadFont(fileName); }
    static void unloadFont() { UnloadFont(font_); }

    // Textures
private:
    static inline std::string const texturePath{"resources/textures/"};

    // Texture atlas
    static inline Texture2D textureAtlas_{};
    static inline std::unordered_map<RenderID, Vector2> texturePositions_{};

public:
    // Texture atlas
    static void loadAtlas(std::string const& filename)
    {
        textureAtlas_ = LoadTexture((texturePath + filename).c_str());
    }

    static Texture2D* getTextureAtlas()
    {
        return &textureAtlas_;
    }

    static void registerTexture(RenderID textureID, Vector2 position)
    {
        texturePositions_.insert(std::make_pair(textureID, position));
    }

    static Vector2& getTexturePosition(RenderID textureID)
    {
        return texturePositions_.at(textureID);
    }

    static void unloadAtlas()
    {
        UnloadTexture(textureAtlas_);
    }
    //=================================

    // Globals
    //=================================
    // Terminate app
private:
    static inline bool isAppClosing_{false};

public:
    static bool shouldAppClose() { return isAppClosing_; }
    static void closeApp() { isAppClosing_ = true; }

    // Active scene
private:
    static inline snx::IScene* activeScene_{};

public:
    static snx::IScene* const& activeScene() { return activeScene_; }
    static void setActiveScene(snx::IScene& scene) { activeScene_ = &scene; }

    // Camera / Viewport
private:
    static inline Camera2D camera_{};

public:
    static Camera2D& camera() { return camera_; }

    // Map size
private:
    static inline RectangleExI mapsize_{};

public:
    static RectangleExI const& mapsize() { return mapsize_; }

    // Extends mapsize to include point provided
    static void extendMapsize(Vector2I V)
    {
        auto left{
            (V.x < mapsize_.left)
                ? V.x
                : mapsize_.left};

        auto right{
            (V.x > mapsize_.right)
                ? V.x
                : mapsize_.right};

        auto top{
            (V.y < mapsize_.top)
                ? V.y
                : mapsize_.top};

        auto bottom{
            (V.y > mapsize_.bottom)
                ? V.y
                : mapsize_.bottom};

        mapsize_ = RectangleExI{
            Vector2I{
                left,
                top},
            Vector2I{
                right,
                bottom}};
    }

    //=================================

    // Configs / settings
    //=================================
    // Debug mode
private:
    static inline bool debugMode_{true};

public:
    static bool debugMode() { return debugMode_; }
    static void toggleDebugMode() { debugMode_ = !debugMode_; }
    //=================================
};

#endif
