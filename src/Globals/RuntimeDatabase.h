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
    static inline std::unordered_map<RenderID, Texture2D> textures_{};
    static inline std::string const texturePath{"resources/textures/"};

public:
    static void loadTexture(RenderID textureID, std::string filename)
    {
        textures_.insert(std::make_pair(textureID, LoadTexture((texturePath + filename).c_str())));
    };

    static Texture2D const* getTexture(RenderID textureID)
    {
        if (textures_.find(textureID) == textures_.end())
            return nullptr;

        return &textures_.at(textureID);
    };

    static void unloadAllTextures()
    {
        for (auto const& texture : textures_)
        {
            Texture2D tex = texture.second;
            UnloadTexture(tex);
        }

        textures_.clear();
    };

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
    static inline snd::IScene* activeScene_{};

public:
    static snd::IScene* const& activeScene() { return activeScene_; }
    static void setActiveScene(snd::IScene& scene) { activeScene_ = &scene; }

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
    };

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
