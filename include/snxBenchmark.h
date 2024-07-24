#ifndef IG20240724124336
#define IG20240724124336

#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace snx
{
    class BM
    {
        static inline std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> starts_;
        static inline std::unordered_map<std::string, std::chrono::nanoseconds::rep> durations_;
        static inline std::unordered_map<std::string, size_t> measurementCounts_;

    public:
        static void start(std::string s = "")
        {
            if (!starts_.contains(s))
            {
                starts_.insert(std::make_pair(s, std::chrono::high_resolution_clock::now()));
                return;
            }

            starts_.at(s) = std::chrono::high_resolution_clock::now();
        }

        static void end(std::string s = "")
        {
            auto end = std::chrono::high_resolution_clock::now();

            if (!durations_.contains(s))
            {
                durations_.insert(
                    std::make_pair(
                        s,
                        (end - starts_[s]).count()));

                measurementCounts_.insert(std::make_pair(
                    s,
                    1));
            }
            else
            {
                durations_.at(s) += (end - starts_[s]).count();
                ++measurementCounts_.at(s);
            }

            auto average = (durations_.at(s) / measurementCounts_.at(s));
            std::cout
                << s
                << ": "
                << average * 10E-9
                << " s\n";
        }
    };

};

#endif