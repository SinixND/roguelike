#include "ActiveScene.h"

#include <iostream>

namespace snd
{
    Scene* ActiveScene::scene_{};

    Scene& ActiveScene::getScene() { return *scene_; }
    void ActiveScene::setScene(Scene& scene) { scene_ = &scene; }

    // Singleton attributes
    //-------------------------------------
    ActiveScene* ActiveScene::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (currentScene_ == nullptr)
        {
            currentScene_ = new ActiveScene();
        }

        return currentScene_;
    }
    ActiveScene* ActiveScene::currentScene_{nullptr};

    std::mutex ActiveScene::mutex_{};

    ActiveScene::ActiveScene() {}
    ActiveScene::~ActiveScene() {}
    //-------------------------------------
}