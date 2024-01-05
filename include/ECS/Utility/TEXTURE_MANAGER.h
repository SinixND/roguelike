#ifndef TEXTUREMANAGER_H_20231217234438
#define TEXTUREMANAGER_H_20231217234438

#include <mutex> // Thread locks
#include <raylib.h>
#include <string>
#include <unordered_map>

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

typedef enum
{
    PLAYER,
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
        static inline TEXTURE_MANAGER* getInstance()
        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (singleton_ == nullptr)
            {
                singleton_ = new TEXTURE_MANAGER();
            }

            return singleton_;
        };

    private:
        static inline TEXTURE_MANAGER* singleton_{nullptr};
        static inline std::mutex mutex_{};

        TEXTURE_MANAGER(){};
        ~TEXTURE_MANAGER(){};

        DISALLOW_COPY_AND_ASSIGN(TEXTURE_MANAGER)
        //=================================
    };

}

#endif
