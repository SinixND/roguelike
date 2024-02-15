#ifndef _20240117233028
#define _20240117233028

#include "RenderId.h"
#include "Scene.h"
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

        static inline Camera2D& camera() { return instance().camera_; };
        static inline void setCamera(Camera2D camera) { instance().camera_ = camera; };
        static inline void setCameraOffset(Vector2 offset) { instance().camera_.offset = offset; };
        static inline void setCameraTarget(Vector2 target) { instance().camera_.target = target; };
        static inline void setCameraZoom(float zoom) { instance().camera_.zoom = zoom; };

    private:
        static inline bool appShouldClose_{false};
        static inline bool mouseActivated_{true};

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

        static inline bool& vSyncMode() { return instance().vSyncMode_; };
        static inline void setVSyncMode(bool status) { vSyncMode_ = status; };

    private:
        static inline bool debugMode_{true};
        static inline bool vSyncMode_{false};
    };
    //=================================

    // Constants
    //=================================
    class Constants : public Singleton<Constants>
    {
    public:
        static inline float tileSize() { return instance().tileSize_; };
        static inline Vector2 tileDimensions() { return Vector2{instance().tileSize_, instance().tileSize_}; };

        static inline void loadFont(const char* fileName) { instance().font_ = LoadFont(fileName); };
        static inline void unloadFont() { UnloadFont(instance().font_); };
        static inline Font& font() { return instance().font_; };

    private:
        static inline const float tileSize_{25};
        static inline Font font_{};
    };
    //=================================

    // Active scene
    //=================================
    class ActiveScene : public Singleton<ActiveScene>
    {
    public:
        static inline snd::Scene& scene() { return *instance().scene_; };
        static inline void setScene(snd::Scene& scene) { instance().scene_ = &scene; };

    private:
        static inline snd::Scene* scene_{};
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
