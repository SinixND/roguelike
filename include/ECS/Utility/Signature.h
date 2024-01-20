#ifndef SIGNATURE_H_20231220225930
#define SIGNATURE_H_20231220225930

#include "Id.h"
#include <string_view>

// Define signature implementation type
//=====================================
// Options:
// BITSET
#define BITSET
//=====================================

#ifdef BITSET
#include <bitset>
#endif

namespace snd
{
    // Set signature type
    //=================================
#ifdef BITSET
    const Id MAX_COMPONENTS = 20;
    using Signature = std::bitset<MAX_COMPONENTS>;
#endif
    //=================================
    /*
        class Signature
        {
        public:
            void set(Id componentTypeId)
            {
    #ifdef BITSET
                signature_.set(componentTypeId);
    #endif
            };

            void reset(Id componentTypeId)
            {
    #ifdef BITSET
                signature_.reset(componentTypeId);
    #endif
            };

            SignatureType& get()
            {
    #ifdef BITSET
                return signature_;
    #endif
            };

            std::string getString() const
            {
    #ifdef BITSET
                return signature_.to_string();
    #endif
            };

            bool check(Id componentTypeId)
            {
    #ifdef BITSET
                return signature_.test(componentTypeId);
    #endif
            };

            bool isSubsetOf(Signature& signature)
            {
    #ifdef BITSET
                return (signature_ & signature.get()) == signature_;
    #endif
            };

        private:
            SignatureType signature_{0};
        };
        */
}

#endif
