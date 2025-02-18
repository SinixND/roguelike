#ifndef IG20250104224727
#define IG20250104224727

#include <cassert>
#include <deque>
#include <string>

int constexpr historySizeMax{ 100 };
int constexpr maxMessagesPerTurn{ 5 };

namespace snx
{
    class Logger
    {
        static inline size_t turn_{ 0 };
        static inline size_t lastTurn_{ 0 };

        static inline std::deque<std::string> turnMessages_{ maxMessagesPerTurn, "" };

        /// History has a fixed size
        static inline std::deque<std::string> history_{ historySizeMax, "" };

    public:
        /// Logs single line message
        static void log(
            std::string const& message
        )
        {
            //* Update history
            if ( lastTurn_ < turn_ )
            {
                lastTurn_ = turn_;

                for ( auto& turnMessage : turnMessages_ )
                {
                    history_.pop_back();
                    history_.push_front( turnMessage );
                    turnMessage = "";
                }
            }

            turnMessages_.pop_back();
            turnMessages_.push_front( "[" + std::to_string( turn_ ) + "] " + message );
        }

        static void logAppend( std::string const& message )
        {
            turnMessages_.front() += message;
        }

        static std::string const& getMessage( size_t index )
        {
            assert( index < turnMessages_.size() && "Max messages per turn exceeded. See Logger.h" );

            return turnMessages_[index];
        }

        static void setTurn( size_t turn )
        {
            turn_ = turn;
        }
    };
}

#endif
