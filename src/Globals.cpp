#include "Globals.h"

#include <raylib.h>

namespace sxd
{
    Color Globals::foreground_{WHITE};
    Color Globals::background_{BLACK};

    bool Globals::debugMode_{true};

    Color& Globals::getForegroundColor() { return foreground_; };
    Color& Globals::getBackgroundColor() { return background_; };

    bool& Globals::getDebugMode() { return debugMode_; };

    // Singleton
    //-------------------------------------
    Globals* Globals::Globals_{nullptr};
    std::mutex Globals::mutex_;

    Globals* Globals::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (Globals_ == nullptr)
        {
            Globals_ = new Globals();
        }

        return Globals_;
    }
    //-------------------------------------
}