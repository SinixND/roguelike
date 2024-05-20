#ifndef IG20240315180832
#define IG20240315180832

#include "Event.h"
#include "ISubscriber.h"
#include <forward_list>
#include <memory>
#include <unordered_map>

// List of subscribers
using SubscriberList = std::forward_list<std::shared_ptr<ISubscriber>>;

// Subject / Publisher / Event / Sender
// Can pushlish multiple events / Can hold multiple subscribers (per event)
class Publisher
{
public:
    // Event is the 'key' that we want to handle.
    // 'subscriber' is the subscriber to be added for the event
    void addSubscriber(std::shared_ptr<ISubscriber> subscriber)
    {
        Event event{subscriber->event};

        // Check if event list exists
        auto it = eventToSubscribers_.find(event);

        if (it == eventToSubscribers_.end())
        {
            eventToSubscribers_[event] = SubscriberList();
        }

        // Add  subscriber to the appropriate list.
        eventToSubscribers_[event].push_front(subscriber);
    }

    void removeSubscriber(std::shared_ptr<ISubscriber> subscriber)
    {
        // Find subscriber to erase
        for (auto& pair : eventToSubscribers_)
        {
            auto& list{pair.second};

            for (auto& entry : list)
            {
                // Remove item if we find it
                if (entry.get() == subscriber.get())
                {
                    list.remove(subscriber);
                    return;
                }
            }
        }
    }

    void notifyAll()
    {
        // Iterate all subscribers
        for (auto& list : eventToSubscribers_)
        {
            for (auto& subscriber : list.second)
            {
                subscriber->onNotify();
            }
        }
    }

    void notify(Event event)
    {
        for (auto& subscriber : eventToSubscribers_[event])
        {
            subscriber->onNotify();
        }
    }

private:
    std::unordered_map<Event, SubscriberList> eventToSubscribers_{{}};
};

#endif
