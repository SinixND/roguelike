#ifndef IG20240601010310
#define IG20240601010310

#include <deque>
#include <string>

int constexpr historySizeMax{100};

namespace snx
{
    class Logger
    {
        static inline std::string stamp_{};

        static inline std::deque<std::string> history_{100, ""};

    public:
        static void log(std::string const& message)
        {
            history_.push_front("[" + stamp_ + "] " + message);

            if (history_.size() > historySizeMax)
            {
                history_.pop_back();
            }
        }

        static void logAppend(std::string const& message)
        {
            history_.front() += message;
        }

        static std::string const& getMessage(int number)
        {
            return history_.at(number);
        }

        static void setStamp(std::string const& stamp)
        {
            stamp_ = stamp;
        }
    };
}

#endif
