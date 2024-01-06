#ifndef CONSTANTS_H_20240105182421
#define CONSTANTS_H_20240105182421

#ifndef __EMSCRIPTEN__
#include <mutex>
#endif
#include "raylib.h"

// Included as an example of a parameterized macro
// Delete assignment operator and copy constructor
#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class CONSTANTS
    {
    public:
        static inline Color& getForegroundColor() { return foreground_; };
        static inline Color& getBackgroundColor() { return background_; };
        static inline Vector2& getTileSize() { return tilesize_; };

    private:
        // Visuals
        static inline Color foreground_{WHITE};
        static inline Color background_{BLACK};

        // Geometrics
        static inline Vector2 tilesize_{25, 25};

        // Singleton attributes
        //=================================
    public:
        // Get Constants instance
        static inline CONSTANTS* getInstance()
        {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(mutex_);
#endif

            if (singleton_ == nullptr)
            {
                singleton_ = new CONSTANTS();
            }

            return singleton_;
        };

    private:
        static inline CONSTANTS* singleton_{nullptr};
#ifndef __EMSCRIPTEN__
        static inline std::mutex mutex_;
#endif

        // Make ctor private
        CONSTANTS(){};
        // Make dtor private
        ~CONSTANTS(){};

        DISALLOW_COPY_AND_ASSIGN(CONSTANTS)
        //=================================
    };
}

#endif
