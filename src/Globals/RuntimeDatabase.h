#ifndef IG20240117233028
#define IG20240117233028

#include "IScene.h"
#include "RenderID.h"
#include "raylibEx.h"
#include "Singleton.h"
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <unordered_map>
#include <utility>

class dtb : public Singleton<dtb>
{
    // CONSTANTS (ONLY SET ONCE)
    //=================================
    // Font
private:
    static inline Font font_{};

public:
    static inline Font& font() { return instance().font_; };
    static inline void loadFont(const char* fileName) { font() = LoadFont(fileName); };
    static inline void unloadFont() { UnloadFont(font()); };

    // Textures
public:
    static inline void loadTexture(RenderID textureID, std::string filename)
    {
        textures().insert(std::make_pair(textureID, LoadTexture((texturePath + filename).c_str())));
    };

    static inline Texture2D* getTexture(RenderID textureID)
    {
        if (textures().find(textureID) == textures().end())
            return nullptr;

        return &textures().at(textureID);
    };

    static inline void unloadAllTextures()
    {
        for (const auto& texture : textures())
        {
            Texture2D tex = texture.second;
            UnloadTexture(tex);
        }
        textures().clear();
    };

private:
    static inline std::unordered_map<RenderID, Texture2D> textures_{};
    static const inline std::string texturePath{"resources/textures/"};

private:
    static inline std::unordered_map<RenderID, Texture2D>& textures() { return instance().textures_; };
    //=================================

    // GLOBALS
    //=================================
    // Terminate app
private:
    static inline bool appShouldClose_{false};

public:
    static inline bool& shouldAppClose() { return instance().appShouldClose_; };
    static inline void closeApp() { shouldAppClose() = true; };

    // Active scene
private:
    static inline snd::IScene* activeScene_{};

public:
    static inline snd::IScene*& activeScene() { return instance().activeScene_; };
    static inline void setActiveScene(snd::IScene& scene) { activeScene() = &scene; };

    // Camera / Viewport
private:
    static inline Camera2D camera_{};

public:
    static inline Camera2D& camera() { return instance().camera_; };
    static inline void moveCamera(Vector2 distance) { camera().target = Vector2Add(camera().target, distance); };

    // Map size
private:
    static inline Area mapSize_{};

public:
    static inline Area& mapSize() { return instance().mapSize_; };

    static inline void updateMapSize(const Vector2i& V)
    {
        auto left{V.x < mapSize().left ? V.x : mapSize().left};
        auto right{V.x > mapSize().right ? V.x : mapSize().right};
        auto top{V.y < mapSize().top ? V.y : mapSize().top};
        auto bottom{V.y > mapSize().bottom ? V.y : mapSize().bottom};

        mapSize() = Area{
            left,
            top,
            right - left + 1,
            bottom - top + 1};
    };

    //=================================

    // CONFIGS / SETTINGS
    //=================================
    // Debug mode
private:
    static inline bool debugMode_{true};

public:
    static inline bool& debugMode() { return instance().debugMode_; };
    static inline void setDebugMode(bool status) { debugMode() = status; };
    //=================================
};

#endif
