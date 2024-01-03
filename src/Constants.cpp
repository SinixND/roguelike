#include "Constants.h"

#include <raylib.h>

namespace snd
{
    Color Constants::foreground_{WHITE};
    Color Constants::background_{BLACK};

    Color& Constants::getForegroundColor() { return foreground_; };
    Color& Constants::getBackgroundColor() { return background_; };

    // Singleton
    //=====================================
    Constants* Constants::globals_{nullptr};
    std::mutex Constants::mutex_;

    Constants* Constants::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (globals_ == nullptr)
        {
            globals_ = new Constants();
        }

        return globals_;
    }
    //=====================================
}