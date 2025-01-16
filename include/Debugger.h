#ifndef IG20240813212155
#define IG20240813212155

#include "GameCamera.h"
#include "Singleton.h"
#include <chrono>
#include <format>
#include <iostream>
#include <raylib.h>
#include <string>

namespace snx
{
    class debug : public snx::Singleton<debug>
    {
        static inline GameCamera gcam_{};

    public:
        static GameCamera& gcam()
        {
            return instance().gcam_;
        }

        template <typename Arg>
        static void cliPrint( Arg message )
        {
            std::cout << message;
        }

        template <typename Arg, typename... Args>
        static void cliPrint(
            Arg message,
            Args... args
        )
        {
            cliPrint( message );
            cliPrint( args... );
            std::cout << "\n";
        }

        template <typename Arg>
        static void cliLog( Arg message )
        {
            auto now = std::chrono::system_clock::now();
            std::string formatted_time = std::format( "{0:%T}", now );
            std::cout << "[" << formatted_time << "] " << message;
        }

        template <typename Arg, typename... Args>
        static void cliLog(
            Arg message,
            Args... args
        )
        {
            cliLog( message );
            cliPrint( args... );
            std::cout << "\n";
        }
    };
}

#endif
