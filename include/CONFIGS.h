#ifndef CONFIGS_H_20240103224203
#define CONFIGS_H_20240103224203

#ifndef __EMSCRIPTEN__
#include <mutex>
#endif

// Included as an example of a parameterized macro
// Delete assignment operator and copy constructor
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

        // Singleton attributes
        //=================================
    public:
        // Get Configs instance
        static inline CONFIGS* getInstance()
        {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(mutex_);
#endif

            if (singleton_ == nullptr)
            {
                singleton_ = new CONFIGS();
            }

            return singleton_;
        };

    private:
        static inline CONFIGS* singleton_{nullptr};
#ifndef __EMSCRIPTEN__
        static inline std::mutex mutex_{};
#endif

        // Make ctor private
        CONFIGS(){};
        // Make dtor private
        ~CONFIGS(){};

        DISALLOW_COPY_AND_ASSIGN(CONFIGS)
        //=================================
    };
}

#endif
