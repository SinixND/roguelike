#ifndef TEXTUREMANAGER_H_20231217234438
#define TEXTUREMANAGER_H_20231217234438

#include <mutex> // thread locks
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
    class TextureManager
    {
    public:
        static void loadTexture(TextureId, std::string filename);
        static Texture2D* retrieveTexture(TextureId);
        static void unloadAllTextures();

        // Singleton attributes
        //---------------------------------
        static TextureManager* getInstance();
        //---------------------------------

    private:
        static std::unordered_map<TextureId, Texture2D> textures_;

        // Singleton attributes
        //---------------------------------
        static TextureManager* singleton_;
        static std::mutex mutex_;
        TextureManager(){};
        ~TextureManager(){};
        DISALLOW_COPY_AND_ASSIGN(TextureManager)
        //---------------------------------
    };

}

#endif
