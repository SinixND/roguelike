#ifndef IG20240601010310
#define IG20240601010310

#include <deque>
#include <string>

int constexpr MAX_HISTORY_SIZE{100};

namespace snx
{
    class Logger
    {
        static inline std::deque<std::string> history_{100, ""};

    public:
        static void log(std::string const& message)
        {
            history_.push_front(message);

            if (history_.size() > MAX_HISTORY_SIZE)
            {
                history_.pop_back();
            }
        };

        static std::string const& getMessage(int number)
        {
            return history_.at(number);
        }
    };
}

#endif