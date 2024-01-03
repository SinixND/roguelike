#ifndef CONFIGS_H_20240103224203
#define CONFIGS_H_20240103224203

#include <mutex>
#include <raylib.h>

// Included as an example of a parameterized macro
// Delete assignment operator and copy constructor
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class Configs
    {
    public:
        static bool getDebugMode();
        static void closeApp();
        static bool shouldAppClose();

        // Singleton attributes
        //=================================
        // Get Configs instance
        static Configs* getInstance();
        //=================================

    private:
        static bool debugMode_;
        static bool appShouldClose_;

        // Singleton attributes
        //=================================
        static Configs* globals_;
        static std::mutex mutex_;

        // Make ctor private
        Configs(){};
        // Make dtor private
        ~Configs(){};

        DISALLOW_COPY_AND_ASSIGN(Configs)
        //=================================
    };
}

#endif
