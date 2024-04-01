#ifndef IG20240401213138
#define IG20240401213138

#include <raylib.h>

class Timer
{
public:
    bool hasTimePassed()
    {
        // Check if passed
        if (dt_ < time_)
        {
            // Tick
            dt_ += GetFrameTime();

            return false;
        }
        else
        {
            dt_ = 0;
            return true;
        }
    }

    bool hasDelayPassed(bool action)
    {
        if (action)
        {
            if (dt_ > time_)
            {
                return true;
            }
            else
            {
                // Tick
                dt_ += GetFrameTime();

                return false;
            }
        }
        else // action not active
        {
            dt_ = 0;

            return false;
        }
    }

    Timer(float time)
        : time_(time)
    {
    }

private:
    float const time_{};
    float dt_{0};
};

#endif