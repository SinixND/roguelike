#ifndef IG20240128195657
#define IG20240128195657

#include <cstddef>
#include <unordered_map>
#include <vector>

namespace snx
{
    template <typename Key>
    class IDenseMap
    {
    public:
        virtual bool contains(Key const& key) const = 0;
        virtual void clear() = 0;
        virtual void erase(Key const& key) = 0;
        // virtual std::vector<Key>& keys() = 0;
        virtual size_t size() const = 0;

        virtual ~IDenseMap() = default;
    };

    template <typename Key, typename Type>
    class DenseMap
        : public IDenseMap<Key>
    {
    public:
        bool contains(Key const& key) const override
        {
            return keyToIndex_.find(key) != keyToIndex_.end();
        }

        // Does NOT overwrite
        void insert(Key const& key, Type const& value = Type{})
        {
            if (contains(key))
            {
                return;
            }

            // Add new key to used keys
            // keys_.insert(key);

            // Get new list index for value
            size_t valueIndex = size();

            // Add new value to list
            values_.push_back(value);

            // Add key to valueIndex mapping
            keyToIndex_.insert(std::make_pair(key, valueIndex));

            // Add valueIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.insert(std::make_pair(valueIndex, key));
        }

        template <typename... Args>
        void emplace(Key const& key, Args&&... args)
        {
            if (contains(key))
            {
                return;
            }

            // Add new key to used keys
            // keys_.insert(key);

            // Get new list index for value
            size_t valueIndex = size();

            // Create and Add new value to list
            values_.push_back(Type{std::forward<Args>(args)...});

            // Add key to valueIndex mapping
            keyToIndex_.insert(std::make_pair(key, valueIndex));

            // Add valueIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.insert(std::make_pair(valueIndex, key));
        }

        Type& operator[](Key const& key)
        {
            insert(key);
            return values_[keyToIndex_[key]];
        }

        void erase(Key const& key) override
        {
            if (!contains(key))
            {
                return;
            }

            // Get list index of removed value
            size_t removedValueIndex{keyToIndex_[key]};
            size_t keptValueIndex{};

            // Replace removed value with last value before popping (if more than one value exists) to keep values contiguous
            if (size() > 1)
            {
                // Get index of (kept) last value that replaces removed value
                keptValueIndex = size() - 1;

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
            indexToKey_.erase(size());

            // Remove key from used keys
            // keys_.erase(key);
        }

        void clear() override
        {
            values_.clear();
            keyToIndex_.clear();
            indexToKey_.clear();
            // keys_.clear();
        }

        std::vector<Type>& values()
        {
            return values_;
        }

        // std::vector<Key>& keys() override
        // {
        // return keys_;
        // }

        size_t size() const override
        {
            return values_.size();
        };

    private:
        // Vector index is used as value key
        std::vector<Type> values_{};

        // Key is used to identify value
        std::unordered_map<Key, size_t> keyToIndex_{};

        // Store a index (value) to key mapping (internal use only)
        std::unordered_map<size_t, Key> indexToKey_{};

        // Vector of all keys in use
        // std::vector<Key> keys_{};
    };
}

#endif
