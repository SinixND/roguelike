#ifndef _20240117233028
#define _20240117233028

#include "RenderID.h"
#include "Scene.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <string>
#include <unordered_map>

// Included as an example of a parameterized macro
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

template <typename Type>
class Singleton
{
public:
    static inline Type& instance()
    {
        static Type instance;
        return instance;
    };

protected:
    Singleton() = default;
    ~Singleton() = default;

    DISALLOW_COPY_AND_ASSIGN(Singleton)
};

class dtb : public Singleton<dtb>
{
    // CONSTANTS (ONLY SET ONCE)
    //=================================
    // Font
public:
    static inline Font& font() { return instance().font_; };
    static inline void loadFont(const char* fileName) { font() = LoadFont(fileName); };
    static inline void unloadFont() { UnloadFont(font()); };

private:
    static inline Font font_{};

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
    static inline std::unordered_map<RenderID, Texture2D>& textures() { return instance().textures_; };
    static const inline std::string texturePath{"resources/textures/"};
    static inline std::unordered_map<RenderID, Texture2D> textures_{};
    //=================================

    // GLOBALS
    //=================================
    // Terminate app
public:
    static inline bool& shouldAppClose() { return instance().appShouldClose_; };
    static inline void closeApp() { shouldAppClose() = true; };

private:
    static inline bool appShouldClose_{false};

    // Camera / Viewport
public:
    static inline Camera2D& camera() { return instance().camera_; };
    static inline void moveCamera(Vector2 distance) { camera().target = Vector2Add(camera().target, distance); };

private:
    static inline Camera2D camera_{};

    // Active scene
public:
    static inline snd::Scene& activeScene() { return *instance().activeScene_; };
    static inline void setActiveScene(snd::Scene& scene) { instance().activeScene_ = &scene; };

private:
    static inline snd::Scene* activeScene_{};
    //=================================

    // CONFIGS / SETTINGS
    //=================================
    // Debug mode
public:
    static inline bool& debugMode() { return instance().debugMode_; };
    static inline void setDebugMode(bool status) { debugMode() = status; };

private:
    static inline bool debugMode_{true};
    //=================================
};

#endif