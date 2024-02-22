#ifndef _20240117233028
#define _20240117233028

#include "RenderID.h"
#include "Scene.h"
#include "raylibEx.h"
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

namespace dtb
{
    namespace
    {
        static inline Font font_{};
    };

    inline void loadFont(const char* fileName) { font_ = LoadFont(fileName); };
    inline Font& font() { return font_; };
    inline void unloadFont() { UnloadFont(font_); };

    // Constants
    //=================================
    class Constants : public Singleton<Constants>
    {
    public:
        static inline Rectangle cursorDeadzone() { return instance().cursorDeadzone_; };
        static inline void setCursorDeadzone(const Rectangle& deadzone) { instance().cursorDeadzone_ = deadzone; };

    private:
        static inline Rectangle cursorDeadzone_{};
    };
    //=================================

    // Application state
    //=================================
    class Globals : public Singleton<Globals>
    {
    public:
        static inline bool& shouldAppClose() { return instance().appShouldClose_; };
        static inline void closeApp() { instance().appShouldClose_ = true; };

        static inline Camera2D& camera() { return instance().camera_; };
        static inline void moveCamera(Vector2 distance) { camera().target = Vector2Add(camera().target, distance); };

        static inline bool pathShown() { return instance().pathShown_; };
        static inline void setPathShown(bool status) { instance().pathShown_ = status; };

    private:
        static inline bool appShouldClose_{false};

        // Setup Camera2D
        static inline Camera2D camera_{};
        static inline bool pathShown_{};
    };
    //=================================

    // Configs / Settings
    //=================================
    class Configs : public Singleton<Configs>
    {
    public:
        static inline bool& debugMode() { return instance().debugMode_; };
        static inline void setDebugMode(bool status) { debugMode_ = status; };

        static inline bool& vSyncMode() { return instance().VSyncMode_; };
        static inline void setVSyncMode(bool status) { VSyncMode_ = status; };

    private:
        static inline bool debugMode_{true};
        static inline bool VSyncMode_{false};
    };
    //=================================

    // Active scene
    //=================================
    class ActiveScene : public Singleton<ActiveScene>
    {
    public:
        static inline snd::Scene& scene() { return *instance().activeScene_; };
        static inline void setScene(snd::Scene& scene) { instance().activeScene_ = &scene; };

    private:
        static inline snd::Scene* activeScene_{};
    };
    //=================================

    // Textures
    //=================================
    class Textures : public Singleton<Textures>
    {
    public:
        static inline void load(RenderID textureID, std::string filename)
        {
            instance().textures_.insert(std::make_pair(textureID, LoadTexture((texturePath + filename).c_str())));
        };

        static inline Texture2D* get(RenderID textureID)
        {
            return &instance().textures_.at(textureID);
        };

        static inline void unloadAll()
        {
            for (const auto& texture : instance().textures_)
            {
                Texture2D tex = texture.second;
                UnloadTexture(tex);
            }
            instance().textures_.clear();
        };

    private:
        static const inline std::string texturePath{"resources/textures/"};
        static inline std::unordered_map<RenderID, Texture2D> textures_{};
    };
    //=================================
}
#endif
