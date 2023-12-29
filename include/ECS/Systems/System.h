#ifndef SYSTEM_H_20231226184152
#define SYSTEM_H_20231226184152

#include "Signature.h"
#include <iostream>

namespace snd
{
    class System
    {
    public:
        Signature& getSignature()
        {
            return signature_;
        };

        System() = default;
        virtual ~System() = default;
        System(const System&) = default;
        System& operator=(const System&) = default;
        System(System&&) = default;
        System& operator=(System&&) = default;

    protected:
        Signature signature_{};
    };
}

#endif
