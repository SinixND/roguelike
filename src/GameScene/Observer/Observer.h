#ifndef IG20240315201533
#define IG20240315201533

#include "Event.h"
#include "IObserver.h"
#include <iostream>

class Subscriber : public ISubscriber
{
public:
    void onNotify() override
    {
        std::cout << "Subscriber " << static_cast<int>(this->event) << " was notified without event\n";
    }

    void onNotify(Event event) override
    {
        std::cout << "Subscriber " << static_cast<int>(this->event) << " was notified with event " << static_cast<int>(event) << "\n";
    }

    Subscriber(Event event)
        : ISubscriber(event){};
};

#endif