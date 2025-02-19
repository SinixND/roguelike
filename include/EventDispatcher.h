#ifndef IG20240527235606
#define IG20240527235606

#include "Events.h"
#include <forward_list>
#include <functional>
#include <unordered_map>

#if defined( DEBUG )
#include "Debugger.h"
#endif

using ListenerList = std::forward_list<std::function<void()>>;

namespace snx
{
    class EventDispatcher
    {
        static inline std::unordered_map<EventId, ListenerList> eventToListenerLists_{ {} };

    public:
        //* Event is the 'key' that we want to handle.
        //* 'listener' is the action (lambda) triggered by the event
        static void addListener(
            EventId event,
            std::function<void()> listener,
            bool notifyOnCreation = false
        )
        {
#if defined( DEBUG )
            debug::cliLog( "Listener added for event '", eventNames.at( event ), "'.\n" );
#endif
            ensureList( event );

            eventToListenerLists_[event].push_front( listener );

            if ( notifyOnCreation )
            {
                notify( event );
            }
        }

        //* Notify (= execute) all listeners for given event
        static void notify( EventId event )
        {
#if defined( DEBUG )
            debug::cliLog( "Notify about event '", eventNames.at( event ), "'.\n" );
#endif
            for ( std::function<void()>& listener : eventToListenerLists_[event] )
            {
                listener();
            }
        }

    private:
        //* Ensure listener list exists for given event
        static void ensureList( EventId event )
        {
            if ( eventToListenerLists_.find( event ) == eventToListenerLists_.end() )
            {
                eventToListenerLists_[event] = ListenerList();
            }
        }
    };
}

#endif
