#include "Configs.h"

#include <raylib.h>

namespace snd
{
    bool Configs::debugMode_{true};
    bool Configs::appShouldClose_{false};

    bool Configs::getDebugMode() { return debugMode_; };
    bool Configs::shouldAppClose() { return appShouldClose_; };
    void Configs::closeApp() { appShouldClose_ = true; };

    // Singleton
    //=====================================
    Configs* Configs::globals_{nullptr};
    std::mutex Configs::mutex_;

    Configs* Configs::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (globals_ == nullptr)
        {
            globals_ = new Configs();
        }

        return globals_;
    }
    //=====================================
}