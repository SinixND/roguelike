#include <memory>
#include <unordered_map>
#include <vector>

enum class EventId;

struct ListenerBaseVirtual
{
    virtual void onNotify() = 0;
    virtual ~ListenerBaseVirtual() = default;
};

using ListenerList = std::vector<std::unique_ptr<ListenerBaseVirtual>>;

struct Listener;

struct Dispatcher
{
    static inline std::unordered_map<EventId, ListenerList> eventToListenerList{};

    static void addListener(
        EventId eventId,
        // ConcreteListener const& concreteListener
        std::unique_ptr<ListenerBaseVirtual> listener
    )
    {
        // eventToListenerList[eventId].push_back( std::make_unique<ConcreteListener>( concreteListener ) );
        eventToListenerList[eventId].push_back( std::move( listener ) );
    }

    // template <typename... Args>
    // static void addListener( EventId eventId, Args... args )
    // {
    //     eventToListenerList[eventId].push_back( std::make_unique<Listener>( Listener( std::forward<Args>( args )... ) ) );
    // }

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

/* Example Usage
#include <functional>
#include <iostream>
/// Have an existing function
void exampleFunction( int a, int b )
{
    std::cout
        << "Do smth with "
        << a
        << " and "
        << b
        << "\n";
}

/// Create  an Event
enum class EventId
{
    EVENTNAME,
};

/// Create a Listener object to store
/// This is basically a functor
struct Listener : public ListenerBaseVirtual
{
    std::function<void( int, int )> wrappedFunction;

    int& a_;
    int& b_;

    Listener( std::function<void( int, int )> func, int& a, int& b )
        : wrappedFunction{ func }
        , a_{ a }
        , b_{ b }
    {
    }

    void onNotify() override
    {
        wrappedFunction( a_, b_ );
    }
};

/// Client code
int main()
{
    int a = 2;
    int b = 3;
    Dispatcher::addListener(
        EventId::EVENTNAME,
        std::make_unique<Listener>(Listener{ exampleFunction, a, b })
    );

    Dispatcher::notify( EventId::EVENTNAME );
}
*/
