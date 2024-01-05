#include "TextureManager.h"

#include <cstring>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <utility>

std::string texturePath{"resources/textures/"};

namespace snd
{
    std::unordered_map<TextureId, Texture2D> TextureManager::textures_{};

    void TextureManager::loadTexture(TextureId textureId, std::string path)
    {
        textures_.insert(std::make_pair(textureId, LoadTexture((texturePath + path).c_str())));
    };

    Texture2D* TextureManager::retrieveTexture(TextureId textureId)
    {
        return &textures_.at(textureId);
    };

    void TextureManager::unloadAllTextures()
    {
        for (const auto& texture : textures_)
        {
            Texture2D tex = texture.second;
            UnloadTexture(tex);
        }
        textures_.clear();
    };

    // Singleton
    //=====================================
    TextureManager* TextureManager::singleton_{nullptr};
    std::mutex TextureManager::mutex_;

    TextureManager* TextureManager::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (singleton_ == nullptr)
        {
            singleton_ = new TextureManager();
        }

        return singleton_;
    }
    //=====================================
}
