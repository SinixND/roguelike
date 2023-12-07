#include "CurrentScene.h"

#include <iostream>

namespace snd
{
    Scene* CurrentScene::scene_{};

    Scene& CurrentScene::getScene() { return *scene_; }
    void CurrentScene::setScene(Scene& scene) { scene_ = &scene; }

    // Singleton attributes
    //-------------------------------------
    CurrentScene* CurrentScene::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::cout << "Request CurrentScene instance...\n";

        if (currentScene_ == nullptr)
        {
            currentScene_ = new CurrentScene();
        }

        return currentScene_;
    }
    CurrentScene* CurrentScene::currentScene_{nullptr};

    std::mutex CurrentScene::mutex_{};

    CurrentScene::CurrentScene() {}
    CurrentScene::~CurrentScene() {}
    //-------------------------------------
}