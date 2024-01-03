#ifndef GLOBALS_H_20231203205506
#define GLOBALS_H_20231203205506

#include <mutex>
#include <raylib.h>

// Included as an example of a parameterized macro
// Delete assignment operator and copy constructor
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class Constants
    {
    public:
        static Color& getForegroundColor();
        static Color& getBackgroundColor();

        // Singleton attributes
        //=================================
        // Get Constants instance
        static Constants* getInstance();
        //=================================

    private:
        static Color foreground_;
        static Color background_;

        // Singleton attributes
        //=================================
        static Constants* globals_;
        static std::mutex mutex_;

        // Make ctor private
        Constants(){};
        // Make dtor private
        ~Constants(){};

        DISALLOW_COPY_AND_ASSIGN(Constants)
        //=================================
    };
}

#endif
