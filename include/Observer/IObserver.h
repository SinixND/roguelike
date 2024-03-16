#ifndef IG20240315181301
#define IG20240315181301

#include "Event.h"

class ISubscriber
{
public:
    Event event{};

    virtual void onNotify() = 0;
    virtual void onNotify(Event event) = 0;

    ISubscriber(Event event)
        : event(event){};

    virtual ~ISubscriber() = default;
};

#endif
