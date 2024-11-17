#ifndef IG20240527235606
#define IG20240527235606

#include "EventId.h"
#include <forward_list>
#include <functional>
#include <unordered_map>

//* List of subscribers (lambdas)
using LambdaVoid = std::function<void()>;
using SubscriberList = std::forward_list<LambdaVoid>;

//* Subject / Publisher / Event / Sender
//* Can pushlish multiple events / Can hold multiple subscribers (per event)
namespace snx
{
    //* Publisher
    class PublisherStatic
    {
        static inline std::unordered_map<EventId, SubscriberList> eventToSubscriberLists_{{}};

    public:
        //* Event is the 'key' that we want to handle.
        //* 'subscriber' is the action triggered by the event
        static void addSubscriber(
            EventId event,
            LambdaVoid subscriber,
            bool fireOnCreation = false)
        {
            ensureList(event);

            eventToSubscriberLists_[event].push_front(subscriber);

            if (fireOnCreation)
            {
                publish(event);
            }
        }

        //* Execute all subscribers for given event
        static void publish(EventId event)
        {
            for (LambdaVoid& subscriber : eventToSubscriberLists_[event])
            {
                subscriber();
            }
        }

        //* Exectue all subscribers event agnostic
        static void publishAll()
        {
            //* Iterate all subscribers
            for (auto& [event, list] : eventToSubscriberLists_)
            {
                notifyAllSubscribers(list);
            }
        }

    private:
        //* Ensure subscriber list exists for given event
        static void ensureList(EventId event)
        {
            if (eventToSubscriberLists_.find(event) == eventToSubscriberLists_.end())
            {
                eventToSubscriberLists_[event] = SubscriberList();
            }
        }

        //* Execute all subscribers in subscriber list
        static void notifyAllSubscribers(std::forward_list<LambdaVoid>& subscriberList)
        {
            for (LambdaVoid& subscriber : subscriberList)
            {
                subscriber();
            }
        }
    };
}

#endif
