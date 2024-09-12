#ifndef IG20240813212155
#define IG20240813212155

#include <chrono>
#include <format>
#include <iostream>
#include <raylib.h>
#include <string>

#include "GameCamera.h"
#include "Singleton.h"

namespace snx
{
    class debug : public snx::Singleton<debug>
    {
        static inline GameCamera gcam_{};

    public:
        static GameCamera& gcam() { return instance().gcam_; }

        template <typename Arg>
        static void cliPrint(Arg arg)
        {
            std::cout << arg;
        }

        template <typename Arg, typename... Args>
        static void cliPrint(Arg arg, Args... args)
        {
            cliPrint(arg);
            cliPrint(args...);
            std::cout << "\n";
        }

        template <typename Arg>
        static void cliLog(Arg arg)
        {
            auto now = std::chrono::system_clock::now();
            std::string formatted_time = std::format("{0:%T}", now);
            std::cout << "[" << formatted_time << "] " << arg;
        }

        template <typename Arg, typename... Args>
        static void cliLog(Arg arg, Args... args)
        {
            cliLog(arg);
            cliPrint(args...);
            std::cout << "\n";
        }
    };
}

#endif
