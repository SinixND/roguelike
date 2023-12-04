#ifndef GLOBALS_H_20231203205506
#define GLOBALS_H_20231203205506

#include <mutex>
#include <raylib.h>

// included as an example of a parameterized macro
// delete assignment operator and copy constructor
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace sxd
{
    class Globals
    {
    public:
        static Color& getForegroundColor();
        static Color& getBackgroundColor();

        static bool& getDebugMode();

        // Singleton attributes
        //---------------------------------
        // get Globals instance
        static Globals* getInstance();
        //---------------------------------

    private:
        static Color foreground_;
        static Color background_;

        static bool darkMode_;
        static bool debugMode_;

        // Singleton attributes
        //---------------------------------
        static Globals* Globals_;
        static std::mutex mutex_;

        // make ctor private
        Globals(){};
        // make dtor private
        ~Globals(){};

        DISALLOW_COPY_AND_ASSIGN(Globals)
        //---------------------------------
    };
}

#endif
