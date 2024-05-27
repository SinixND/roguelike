#ifndef IG20240527235606
#define IG20240527235606

#include "Event.h"
#include <forward_list>
#include <functional>
#include <unordered_map>

// List of subscribers (lambdas)
using SubscriberList = std::forward_list<std::function<void()>>;

// Subject / Publisher / Event / Sender
// Can pushlish multiple events / Can hold multiple subscribers (per event)
class Publisher
{
public:
    // Event is the 'key' that we want to handle.
    // 'subscriber' is the subscriber to be added for the event
    static void addSubscriber(Event event, std::function<void()> subscriber)
    {
        // Check if event list exists
        auto it = eventToSubscribers_.find(event);

        if (it == eventToSubscribers_.end())
        {
            eventToSubscribers_[event] = SubscriberList();
        }

        // Add  subscriber to the appropriate list.
        eventToSubscribers_[event].push_front(subscriber);
    }

    static void notifyAll()
    {
        // Iterate all subscribers
        for (auto& list : eventToSubscribers_)
        {
            for (auto& subscriber : list.second)
            {
                subscriber();
            }
        }
    }

    static void notify(Event event)
    {
        for (auto& subscriber : eventToSubscribers_[event])
        {
            subscriber();
        }
    }

private:
    static inline std::unordered_map<Event, SubscriberList> eventToSubscribers_{{}};
};

#endif
