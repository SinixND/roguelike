#ifndef SIGNATURE_H_20231220225930
#define SIGNATURE_H_20231220225930

#include "Id.h"
#include <bitset>

namespace snd
{
    constexpr Id MAX_COMPONENTS = 10;

    using Signature = std::bitset<MAX_COMPONENTS>;
}

#endif