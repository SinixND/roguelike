#ifndef _20240128195657
#define _20240128195657

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
        virtual void clear() = 0;
        virtual bool contains(const Key& key) = 0;
        virtual std::unordered_set<Key>& getKeys() = 0;
        virtual ~ISparseSet() = default;
    };

    template <typename Key, typename Type>
    class SparseSet
        : public ISparseSet<Key>
    {
    public:
        // Create non-existing, update existing
        Type* update(const Key& key, const Type& value)
        {
            if (contains(key))
            {
                values_[keyToIndex_[key]] = value;
                return &values_[keyToIndex_[key]];
            }

            // Add new key to used keys
            keys_.insert(key);

            // Get new list index for value
            Index valueIndex = values_.size();

            // Add new value to list
            values_.push_back(value);

            // Add key to valueIndex mapping
            keyToIndex_.insert(std::make_pair(key, valueIndex));

            // Add valueIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.insert(std::make_pair(valueIndex, key));

            return &values_[keyToIndex_[key]];
        };

        // Create non-exisitng, skip exisitng
        Type* insert(const Key& key, const Type& value)
        {
            if (contains(key))
                return nullptr;

            return update(key, value);
        };

        void erase(const Key& key) override
        {
            if (!contains(key))
                return;

            // Get list index of removed value
            Index removedValueIndex{keyToIndex_[key]};
            Index keptValueIndex{};

            // Replace removed value with last value before popping (if more than one value exists) to keep values contiguous
            if (values_.size() > 1)
            {
                // Get index of (kept) last value that replaces removed value
                keptValueIndex = values_.size() - 1;

                // Get key of replacing/kept value
                Key keptkey = indexToKey_[keptValueIndex];

                // Replace (removed) value with kept value (by index) so last entry (duplicate) can be popped (becomes new removed)
                values_[removedValueIndex] = values_[keptValueIndex];

                // Update value index after replacement
                keptValueIndex = removedValueIndex;

                // Update key to valueIndex mapping for kept key
                keyToIndex_[keptkey] = keptValueIndex;

                // Update index to key mapping for kept key
                indexToKey_[keptValueIndex] = keptkey;
            }

            // Remove removed key from mapping
            keyToIndex_.erase(key);

            // Remove (duplicate) last value
            values_.pop_back();

            // Remove removed value from mapping
            indexToKey_.erase(values_.size());

            // Remove key from used keys
            keys_.erase(key);
        };

        void clear() override
        {
            for (const auto& key : keys_)
            {
                erase(key);
            }
        };

        bool contains(const Key& key) override
        {
            return keys_.contains(key);
        };

        Type* at(const Key& key)
        {
            if (!contains(key))
                return nullptr;

            return &values_[keyToIndex_[key]];
        };

        std::vector<Type>& getValues()
        {
            return values_;
        };

        std::unordered_set<Key>& getKeys() override
        {
            return keys_;
        };

    private:
        std::vector<Type> values_{};                  // Vector index is used as value key
        std::unordered_map<Key, Index> keyToIndex_{}; // Key is used to identify value
        std::unordered_map<Index, Key> indexToKey_{}; // Store a index (value) to key mapping (internal use only)
        std::unordered_set<Key> keys_{};              // Set of all keys in use
    };
}

#endif
