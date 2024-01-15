#ifndef TEXTUREMANAGER_H_20231217234438
#define TEXTUREMANAGER_H_20231217234438

#include <raylib.h>
#include <string>
#include <unordered_map>

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

typedef enum
{
    PLAYER_TEXTURE,
    CURSOR_TEXTURE,
    WALL_TEXTURE,
    FLOOR_TEXTURE,
} TextureId;

namespace snd
{
    class TEXTURE_MANAGER
    {
    public:
        static inline void loadTexture(TextureId textureId, std::string filename)
        {
            textures_.insert(std::make_pair(textureId, LoadTexture((texturePath + filename).c_str())));
        };

        static inline Texture2D* retrieveTexture(TextureId textureId)
        {
            return &textures_.at(textureId);
        };

        static inline void unloadAllTextures()
        {
            for (const auto& texture : textures_)
            {
                Texture2D tex = texture.second;
                UnloadTexture(tex);
            }
            textures_.clear();
        };

    private:
        static const inline std::string texturePath{"resources/textures/"};
        static inline std::unordered_map<TextureId, Texture2D> textures_{};

        // Singleton attributes
        //=================================
    public:
        // Get Singleton instance
        static inline TEXTURE_MANAGER& get()
        {
            static TEXTURE_MANAGER singleton;
            return singleton;
        };

    private:
        TEXTURE_MANAGER() = default;
        ~TEXTURE_MANAGER() = default;

        DISALLOW_COPY_AND_ASSIGN(TEXTURE_MANAGER)
        //=================================
    };
}

#endif
