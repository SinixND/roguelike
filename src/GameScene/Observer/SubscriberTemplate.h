#ifndef IG20240315201533
#define IG20240315201533

#include "Event.h"
#include "ISubscriber.h"
#include <iostream>

class Subscriber : public ISubscriber
{
public:
    void onNotify() override
    {
        std::cout << "Subscriber to event " << static_cast<int>(this->event) << " was notified\n";
    }

    Subscriber(Event event)
        : ISubscriber(event){};
};

#endif