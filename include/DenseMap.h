#ifndef IG20240128195657
#define IG20240128195657

#include <cassert>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace snx
{
    template <typename Key>
    struct IDenseMap
    {
        virtual bool contains(Key const& key) const = 0;
        virtual void clear() = 0;
        virtual void erase(Key const& key) = 0;
        // virtual std::vector<Key> const& keys() = 0;
        virtual size_t size() const = 0;

        virtual ~IDenseMap() = default;
    };

    template <typename Key, typename Type>
    class DenseMap
        : public IDenseMap<Key>
    {
    public:
        // ITERATORS
        auto begin()
        {
            return values_.begin();
        }

        auto end()
        {
            return values_.end();
        }

        // CAPACITY
        // Return size of DenseMap
        size_t size() const override
        {
            return values_.size();
        }

        // MODIFIERS
        // Does NOT overwrite existing
        Type const& insert(Key const& key, Type const& value = Type{})
        {
            if (contains(key))
            {
                return at(key);
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
            assert((keyToIndex_.size() == indexToKey_.size()) && "DenseMap mismatch!");
            return at(key);
        }

        // Does NOT overwrite existing
        template <typename... Args>
        Type const& emplace(Key const& key, Args&&... args)
        {
            if (contains(key))
            {
                return at(key);
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
            assert((keyToIndex_.size() == indexToKey_.size()) && "DenseMap mismatch!");
            return at(key);
        }

        // Moves last value to gap
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

        // Empty all containers
        void clear() override
        {
            values_.clear();
            keyToIndex_.clear();
            indexToKey_.clear();
            // keys_.clear();
        }

        // LOOKUP
        // Access
        Type const& at(Key const& key) const
        {
            return values_.at(keyToIndex_.at(key));
        }

        // Allow non-const call
        Type& at(Key const& key)
        {
            return const_cast<Type&>(std::as_const(*this).at(key));
        }

        // Access or insert
        Type& operator[](Key const& key)
        {
            insert(key);
            return at(key);
        }

        bool contains(Key const& key) const override
        {
            return keyToIndex_.find(key) != keyToIndex_.end();
        }

        // Return vector (contiguous memory)
        std::vector<Type> const& values() const
        {
            return values_;
        }
        // Optional operator
        // std::vector<Type> const& operator()() const {
        // return values_;
        // }

        // Allow non-const call
        std::vector<Type>& values()
        {
            return const_cast<std::vector<Type>&>(std::as_const(*this).values());
        }
        // Optional operator
        // std::vector<Type>& operator()() {
        // return const_cast<std::vector<Type>&>(std::as_const(*this).operator());
        // }

        // std::vector<Key> const& keys() override
        // {
        // return keys_;
        // }

    private:
        // Vector index is used as value key
        std::vector<Type>
            values_{};

        // Key is used to identify value
        std::unordered_map<Key, size_t> keyToIndex_{};

        // Store a index (value) to key mapping (internal use only)
        std::unordered_map<size_t, Key> indexToKey_{};

        // Vector of all keys in use
        // std::vector<Key> keys_{};
    };
}

#endif
