#ifndef SIGNATURE_H_20231220225930
#define SIGNATURE_H_20231220225930

#include "Id.h"
#include <bitset>

namespace snd
{
    constexpr Id MAX_COMPONENTS = 10;

    using Signature = std::bitset<MAX_COMPONENTS>;

    /*
        class Signature
        {
            // std::bitset
            // ============================
        public:
            void set(Id componentTypeId)
            {
                signature_.set(componentTypeId);
            };

            void reset(Id componentTypeId)
            {
                signature_.reset(componentTypeId);
            };

            constexpr SignatureType& retrieve()
            {
                return signature_;
            };

            constexpr bool check(Id componentTypeId)
            {
                return signature_.test(componentTypeId);
            };

            constexpr bool isSubsetOf(const Signature& signature)
            {
                return (signature_ & signature.retrieve()) == signature_;
            };

            constexpr bool isSupersetOf(const Signature& signature)
            {
                return (signature_ & signature.retrieve()) == signature.retrieve();
            };

            constexpr bool isEqualTo(Signature signature)
            {
                return (signature_ == signature.retrieve());
            };

            constexpr bool isEmpty()
            {
                return (signature_ == 0);
            };

        private:
            SignatureType signature_{0};
            // ============================
        };
        */
}

#endif
