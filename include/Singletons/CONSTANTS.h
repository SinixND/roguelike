#ifndef CONSTANTS_H_20240105182421
#define CONSTANTS_H_20240105182421

#include <raylib.h>

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
        // Get Singleton instance
        static inline CONSTANTS& get()
        {
            static CONSTANTS singleton;
            return singleton;
        };

    private:
        CONSTANTS() = default;
        ~CONSTANTS() = default;

        DISALLOW_COPY_AND_ASSIGN(CONSTANTS)
        //=================================
    };
}

#endif
