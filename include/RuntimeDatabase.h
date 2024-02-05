#ifndef DATABASE_H_20240117233028
#define DATABASE_H_20240117233028

#include "Scene.h"
#include <raylibEx.h>
#include <string>

// Included as an example of a parameterized macro
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

template <typename Type>
class Singleton
{
public:
    static inline Type& getInstance()
    {
        static Type instance;
        return instance;
    };

protected:
    Singleton() = default;
    ~Singleton() = default;

    DISALLOW_COPY_AND_ASSIGN(Singleton)
};

enum RenderId
{
    PLAYER,
    CURSOR,
    WALL_TILE,
    FLOOR_TILE,
    REACHABLE_TILE,
    PATH_TILE,
    ATTACKABLE_TILE,
    SUPPORTABLE_TILE,
};

namespace dtb
{
    // Application state
    //=================================
    class State : public Singleton<State>
    {
    public:
        static inline bool& shouldAppClose() { return getInstance().appShouldClose_; };
        static inline void closeApp() { getInstance().appShouldClose_ = true; };

        static inline bool isMouseActivated() { return getInstance().mouseActivated_; }
        static inline void toggleMouseActivated()
        {
            getInstance().mouseActivated_ = !getInstance().mouseActivated_;

            if (getInstance().mouseActivated_)
            {
                ShowCursor();
            }
            else
            {
                HideCursor();
            }
        }

    private:
        static inline bool appShouldClose_{false};
        static inline bool mouseActivated_{true};
    };
    //=================================

    // Configs / Settings
    //=================================
    class Configs : public Singleton<Configs>
    {
    public:
        static inline bool& getDebugMode() { return getInstance().debugMode_; };
        static inline bool& getVSyncMode() { return getInstance().vSyncMode_; };

    private:
        static inline bool debugMode_{true};
        static inline bool vSyncMode_{false};
    };
    //=================================

    // Constants
    //=================================
    struct Constants : public Singleton<Constants>
    {
        // Visuals
        static inline const Color foregroundColor_{WHITE};
        static inline const Color backgroundColor_{BLACK};

        // Geometrics
        static inline const Vector2 tileSize_{25, 25};
    };
    //=================================

    // Active scene
    //=================================
    class ActiveScene : public Singleton<ActiveScene>
    {
    public:
        static inline snd::Scene& getScene() { return *getInstance().scene_; };
        static inline void setScene(snd::Scene& scene) { getInstance().scene_ = &scene; };

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
            getInstance().textures_.insert(std::make_pair(textureId, LoadTexture((texturePath + filename).c_str())));
        };

        static inline Texture2D* get(RenderId textureId)
        {
            return &getInstance().textures_.at(textureId);
        };

        static inline void unloadAll()
        {
            for (const auto& texture : getInstance().textures_)
            {
                Texture2D tex = texture.second;
                UnloadTexture(tex);
            }
            getInstance().textures_.clear();
        };

    private:
        static const inline std::string texturePath{"resources/textures/"};
        static inline std::unordered_map<RenderId, Texture2D> textures_{};
    };
    //=================================
    //
}
#endif
