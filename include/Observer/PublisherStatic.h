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
namespace snx
{
    // Publisher
    class Publisher
    {
        static inline std::unordered_map<Event, SubscriberList> eventToSubscriberLists_{{}};

    public:
        // Event is the 'key' that we want to handle.
        // 'subscriber' is the action triggered by the event
        static void addSubscriber(Event event, std::function<void()> subscriber)
        {
            ensureList(event);

            eventToSubscriberLists_[event].push_front(subscriber);
        }

        // Execute all subscribers for given event
        static void publish(Event event)
        {
            for (auto& subscriber : eventToSubscriberLists_[event])
            {
                subscriber();
            }
        }

        // Exectue all subscribers event agnostic
        static void publishAll()
        {
            // Iterate all subscribers
            for (auto& mapping : eventToSubscriberLists_)
            {
                notifyAllSubscribers(mapping.second);
            }
        }

    private:
        // Ensure subscriber list exists for given event
        static void ensureList(Event event)
        {
            if (eventToSubscriberLists_.find(event) == eventToSubscriberLists_.end())
            {
                eventToSubscriberLists_[event] = SubscriberList();
            }
        }

        // Execute all subscribers in subscriber list
        static void notifyAllSubscribers(std::forward_list<std::function<void()>>& subscriberList)
        {
            for (auto& subscriber : subscriberList)
            {
                subscriber();
            }
        }
    };
}

#endif
