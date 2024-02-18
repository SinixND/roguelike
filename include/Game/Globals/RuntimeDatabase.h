#ifndef _20240117233028
#define _20240117233028

#include "RenderId.h"
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
    // Application state
    //=================================
    class Globals : public Singleton<Globals>
    {
    public:
        static inline bool& shouldAppClose() { return instance().appShouldClose_; };
        static inline void closeApp() { instance().appShouldClose_ = true; };

        static inline Camera2D& camera() { return instance().camera_; };
        static inline void moveCamera(Vector2 distance) { camera().target = Vector2Add(camera().target, distance); };

    private:
        static inline bool appShouldClose_{false};

        // Setup Camera2D
        static inline Camera2D camera_{};
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

        static inline bool isMouseActivated() { return instance().mouseActivated_; }
        static inline void toggleMouseActivated()
        {
            instance().mouseActivated_ = !instance().mouseActivated_;

            if (instance().mouseActivated_)
            {
                ShowCursor();
            }
            else
            {
                HideCursor();
            }
        }

    private:
        static inline bool debugMode_{true};
        static inline bool VSyncMode_{false};
        static inline bool mouseActivated_{false};
    };
    //=================================

    // Constants
    //=================================
    class Constants : public Singleton<Constants>
    {
    public:
        static inline void loadFont(const char* fileName) { instance().font_ = LoadFont(fileName); };
        static inline Font& font() { return instance().font_; };
        static inline void unloadFont() { UnloadFont(instance().font_); };

        static inline Rectangle cursorDeadzone() { return instance().cursorDeadzone_; };
        static inline void setCursorDeadzone(const Rectangle& deadzone) { instance().cursorDeadzone_ = deadzone; };

    private:
        static inline Font font_{};
        static inline Rectangle cursorDeadzone_{};
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
        static inline void load(RenderId textureId, std::string filename)
        {
            instance().textures_.insert(std::make_pair(textureId, LoadTexture((texturePath + filename).c_str())));
        };

        static inline Texture2D* get(RenderId textureId)
        {
            return &instance().textures_.at(textureId);
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
        static inline std::unordered_map<RenderId, Texture2D> textures_{};
    };
    //=================================
}
#endif
