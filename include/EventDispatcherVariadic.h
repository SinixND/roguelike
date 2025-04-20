#include <memory>
#include <unordered_map>
#include <vector>

enum class EventId;

class IListener
{
public:
    virtual void onNotify() = 0;
    virtual ~IListener() = default;
};

using ListenerList = std::vector<std::unique_ptr<IListener>>;

class ConcreteListener : public IListener
{
};

class Dispatcher
{
public:
    static inline std::unordered_map<EventId, ListenerList> eventToListenerList{};

    static void addListener(
        EventId eventId,
        std::unique_ptr<IListener> listener
    )
    {
        eventToListenerList[eventId].push_back( std::move( listener ) );
    }

    static void notify( EventId eventId )
    {
        if ( !eventToListenerList.contains( eventId ) )
        {
            return;
        }

        for ( auto& listener : eventToListenerList.at( eventId ) )
        {
            listener->onNotify();
        }
    }
};

