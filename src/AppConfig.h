#ifndef IG20240529010632
#define IG20240529010632

#include <raylib.h>

//* Configs imported from user file

class AppConfig
{
    //* Window dimensions
    int windowWidth_{800};
    int windowHeight_{940};

    //* Fps
    bool showFPS_{true};
    bool vSync_{false};

public:
    int windowWidth() const
    {
        return windowWidth_;
    }

    int windowHeight() const
    {
        return windowHeight_;
    }

    bool showFPS() const
    {
        return showFPS_;
    }
    bool vSync() const
    {
        return vSync_;
    }
};

#endif
