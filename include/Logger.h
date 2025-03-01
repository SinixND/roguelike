#ifndef IG20250104224727
#define IG20250104224727

#include <cassert>
#include <cstring>
#include <deque>
#include <string>

int constexpr historySizeMax{ 100 };
int constexpr maxMessagesPerTurn{ 5 };

namespace snx
{
    class Logger
    {
        static inline size_t turn_{ 1 };
        static inline size_t lastTurn_{ 0 };

        static inline std::deque<std::string> turnMessages_{ maxMessagesPerTurn, "" };

        /// History has a fixed size
        static inline std::deque<std::string> history_{ historySizeMax, "" };

    public:
        static void updateHistory()
        {
            if ( lastTurn_ == ( turn_ - 1 ) )
            {
                lastTurn_ = turn_ - 1;

                for ( auto& turnMessage : turnMessages_ )
                {
                    history_.pop_back();
                    history_.push_front( turnMessage );
                    turnMessage = "";
                }
            }
        }

        /// Logs single line message
        static void log(
            std::string const& message
        )
        {
            lastTurn_ = turn_;

            turnMessages_.pop_back();

            turnMessages_.push_front( "[" + std::to_string( turn_ ) + "] " + message );
        }

        static void logAppend( std::string const& message )
        {
            turnMessages_.front() += message;
        }

        static size_t hasTurnMessage()
        {
            return turnMessages_.size();
        }

        static std::string const& getMessage( size_t idx )
        {
            assert( idx < turnMessages_.size() && "Max messages per turn exceeded. See Logger.h" );

            return turnMessages_[idx];
        }

        static void incrementTurn()
        {
            updateHistory();
            ++turn_;
        }
    };
}

#endif
