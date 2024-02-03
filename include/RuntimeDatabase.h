#ifndef DATABASE_H_20240117233028
#define DATABASE_H_20240117233028

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

typedef enum
{
    PLAYER,
    CURSOR,
    WALL_TILE,
    FLOOR_TILE,
    REACHABLE_TILE,
    PATH_TILE,
    ATTACKABLE_TILE,
    SUPPORTABLE_TILE,
} TextureId;

namespace dtb
{
    // Configs / Settings
    //=================================
    class Configs : public Singleton<Configs>
    {
    public:
        // Global debug flag
        static inline bool dbg{false};

    public:
        static inline bool& getDebugMode() { return getInstance().debugMode_; };

        static inline bool& shouldAppClose() { return getInstance().appShouldClose_; };
        static inline void closeApp() { getInstance().appShouldClose_ = true; };

        static inline bool& areTilesShown() { return getInstance().tilesShown_; };
        static inline void showTiles() { getInstance().tilesShown_ = true; };
        static inline void hideTiles() { getInstance().tilesShown_ = false; };

        static inline bool& isPathShown() { return getInstance().pathShown_; };
        static inline void showPath() { getInstance().pathShown_ = true; };
        static inline void hidePath() { getInstance().pathShown_ = false; };

        static inline int& getCurrentLevel() { return getInstance().currentLevel_; }
        static inline void increaseLevel() { getInstance().currentLevel_++; }

        static inline bool isMouseActivated() { return mouseActivated_; }
        static inline void toggleMouseActivated()
        {
            mouseActivated_ = !mouseActivated_;

            if (mouseActivated_)
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
        static inline bool appShouldClose_{false};
        static inline bool tilesShown_{false};
        static inline bool pathShown_{false};
        static inline int currentLevel_{0};
        static inline bool mouseActivated_{true};
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
        static inline void load(TextureId textureId, std::string filename)
        {
            getInstance().textures_.insert(std::make_pair(textureId, LoadTexture((texturePath + filename).c_str())));
        };

        static inline Texture2D* get(TextureId textureId)
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
        static inline std::unordered_map<TextureId, Texture2D> textures_{};
    };
    //=================================
    //
}
#endif
