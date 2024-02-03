#ifndef SPARSESET_H_20240128195657
#define SPARSESET_H_20240128195657

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace snd
{
    typedef size_t Index;

    template <typename Key>
    class ISparseSet
    {
    public:
        virtual void erase(const Key& key) = 0;
        virtual void eraseAll() = 0;
        virtual bool test(const Key& key) = 0;
        virtual std::unordered_set<Key>* getAllKeys() = 0;
        virtual ~ISparseSet() = default;
    };

    template <typename Key, typename Type>
    class SparseSet
        : public ISparseSet<Key>
    {
    public:
        // Create non-existing, update existing
        void insert(const Key& key, const Type& element)
        {
            if (test(key))
            {
                elements_[keyToIndex_[key]] = element;
                return;
            }

            // Add new key to used keys
            keys_.insert(key);

            // Get new list index for element
            Index elementIndex = elements_.size();

            // Add new element to list
            elements_.push_back(element);

            // Add key to elementIndex mapping
            keyToIndex_.insert(std::make_pair(key, elementIndex));

            // Add elementIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.insert(std::make_pair(elementIndex, key));
        };

        void erase(const Key& key) override
        {
            if (!test(key))
            {
                return;
            };

            // Get list index of removed element
            Index removedElementIndex{keyToIndex_[key]};
            Index keptElementIndex{};

            // Replace removed element with last element before popping (if more than one element exists) to keep elements contiguous
            if (elements_.size() > 1)
            {
                // Get index of (kept) last element that replaces removed element
                keptElementIndex = elements_.size() - 1;

                // Get key of replacing/kept element
                Key keptkey = indexToKey_[keptElementIndex];

                // Replace (removed) element with kept element (by index) so last entry (duplicate) can be popped (becomes new removed)
                elements_[removedElementIndex] = elements_[keptElementIndex];

                // Update element index after replacement
                keptElementIndex = removedElementIndex;

                // Update key to elementIndex mapping for kept key
                keyToIndex_[keptkey] = keptElementIndex;

                // Update index to key mapping for kept key
                indexToKey_[keptElementIndex] = keptkey;
            }

            // Remove removed key from mapping
            keyToIndex_.erase(key);

            // Remove (duplicate) last element
            elements_.pop_back();

            // Remove removed element from mapping
            indexToKey_.erase(elements_.size());

            // Remove key from used keys
            keys_.erase(key);
        };

        void eraseAll() override
        {
            for (const auto& key : keys_)
            {
                erase(key);
            }
        };

        bool test(const Key& key) override
        {
            return keys_.contains(key);
        };

        Type* get(const Key& key)
        {
            if (!test(key))
                return nullptr;

            return &elements_[keyToIndex_[key]];
        };

        std::vector<Type>* getAllElements()
        {
            return &elements_;
        };

        const Key* getFirstKey()
        {
            if (keys_.empty())
                return nullptr;

            return &(*keys_.begin());
        };

        std::unordered_set<Key>* getAllKeys() override
        {
            return &keys_;
        };

    private:
        std::vector<Type> elements_{};                // Vector index is used as element key
        std::unordered_map<Key, Index> keyToIndex_{}; // Key is used to identify element
        std::unordered_map<Index, Key> indexToKey_{}; // Store a index (element) to key mapping (internal use only)
        std::unordered_set<Key> keys_{};              // Set of all keys in use
    };
}

#endif
