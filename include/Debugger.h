#ifndef IG20240813212155
#define IG20240813212155

#include <chrono>
#include <format>
#include <iostream>
#include <raylib.h>
#include <string>

#include "Singleton.h"
namespace snx
{
    struct dbg : public snx::Singleton<dbg>
    {
        static inline Camera2D cam_{};

    public:
        static Camera2D& cam() { return instance().cam_; }

        static void cliLog(std::string msg)
        {
            auto now = std::chrono::system_clock::now();
            std::string formatted_time = std::format("{0:%T}", now);
            std::cout << formatted_time << ": " << msg << std::endl;
        }
    };
}

#endif