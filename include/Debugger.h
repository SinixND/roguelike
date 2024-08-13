#ifndef IG20240813212155
#define IG20240813212155

#include <chrono>
#include <format>
#include <iostream>
#include <string>

namespace snx
{
    inline void debug(std::string msg)
    {
        auto now = std::chrono::system_clock::now();
        std::string formatted_time = std::format("{0:%T}", now);
        std::cout << formatted_time << ": " << msg << std::endl;
    }
}

#endif