#ifndef CONFIGS_H_20240103224203
#define CONFIGS_H_20240103224203

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class CONFIGS
    {
    public:
        static inline bool getDebugMode() { return debugMode_; };
        static inline bool shouldAppClose() { return appShouldClose_; };
        static inline void closeApp() { appShouldClose_ = true; };

    private:
        static inline bool debugMode_{true};
        static inline bool appShouldClose_{false};
        static inline int seed_{};

        // Singleton attributes
        //=================================
    public:
        // Get Singleton instance
        static inline CONFIGS& get()
        {
            static CONFIGS singleton;
            return singleton;
        };

    private:
        CONFIGS() = default;
        ~CONFIGS() = default;

        DISALLOW_COPY_AND_ASSIGN(CONFIGS)
        //=================================
    };
}

#endif
