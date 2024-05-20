#ifndef IG20240315181301
#define IG20240315181301

#include "Event.h"

// Observer / Subscriber / Listener / Receiver
// Only subscribes to one event
class ISubscriber
{
public:
    Event event{};

    virtual void onNotify() = 0;

    ISubscriber(Event event)
        : event(event){};

    virtual ~ISubscriber() = default;
};

#endif
