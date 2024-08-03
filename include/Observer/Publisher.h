#ifndef IG20240803133300
#define IG20240803133300

#include "Event.h"
#include <forward_list>
#include <functional>
#include <unordered_map>

namespace snx
{
    // List of subscribers (lambdas)
    template<typename FType>
    using SubList = std::forward_list<FType>;

    // Subject / Publisher / Event / Sender
    // Can pushlish multiple events / Can hold multiple subscribers (per event)

    // Publisher
    template<typename FType>
    class Publisher
    {
        std::unordered_map<Event, SubList<FType>> eventToSubscriberLists_{{}};

    public:
        // Event is the 'key' that we want to handle.
        // 'subscriber' is the action triggered by the event
        void addSubscriber(Event event, std::function<void()> subscriber, bool fireOnCreation = false)
        {
            ensureList(event);

            eventToSubscriberLists_[event].push_front(subscriber);

            if (fireOnCreation)
            {
                publish(event);
            }
        }

        // Execute all subscribers for given event
        void publish(Event event)
        {
            for (auto& subscriber : eventToSubscriberLists_[event])
            {
                subscriber();
            }
        }

        // Exectue all subscribers event agnostic
        void publishAll()
        {
            // Iterate all subscribers
            for (auto& mapping : eventToSubscriberLists_)
            {
                notifyAllSubscribers(mapping.second);
            }
        }

    private:
        // Ensure subscriber list exists for given event
        void ensureList(Event event)
        {
            if (eventToSubscriberLists_.find(event) == eventToSubscriberLists_.end())
            {
                eventToSubscriberLists_[event] = SubList<FType>();
            }
        }

        // Execute all subscribers in subscriber list
        void notifyAllSubscribers(SubList<FType>& subscriberList)
        {
            for (auto& subscriber : subscriberList)
            {
                subscriber();
            }
        }
    };
}

#endif
