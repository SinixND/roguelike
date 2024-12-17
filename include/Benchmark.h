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
        static void start(std::string bmName = "")
        {
            if (!starts_.contains(bmName))
            {
                starts_.insert({bmName, std::chrono::high_resolution_clock::now()});
                return;
            }

            starts_.at(bmName) = std::chrono::high_resolution_clock::now();
        }

        static void stop(std::string bmName = "")
        {
            auto end = std::chrono::high_resolution_clock::now();

            if (!durations_.contains(bmName))
            {
                durations_.insert({bmName, (end - starts_[bmName]).count()});

                measurementCounts_.insert({bmName, 1});
            }

            else
            {
                durations_.at(bmName) += (end - starts_[bmName]).count();
                ++measurementCounts_.at(bmName);
            }
        }

        static void report(std::string bmName = "")
        {
            auto average = (durations_.at(bmName) / measurementCounts_.at(bmName));
            std::cout
                << "\""
                << bmName
                << "\""
                << ": "
                << average * 10E-6
                << " ms / "
                << 1 / (average * 10E-9)
                << " 1/s over "
                << measurementCounts_.at(bmName)
                << " iterations\n";
        }

        static void stopAndReport(std::string bmName = "")
        {
            stop(bmName);
            report(bmName);
        }
    };
};

#endif
