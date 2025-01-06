#ifndef IG20250109220707
#define IG20250109220707

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

namespace snx
{
    template <typename Type>
    class SparseSet
    {
    public:
        //* ITERATORS
        auto begin() const
        {
            return values_.begin();
        }

        auto begin()
        {
            return values_.begin();
        }

        auto end() const
        {
            return values_.end();
        }

        auto end()
        {
            return values_.end();
        }

        //* CAPACITY
        //* Return size of SparseSet
        size_t size() const
        {
            return values_.size();
        }

        //* MODIFIERS
        Type* assign(
            size_t key,
            Type const& value = Type{})
        {
            if (keyToIndex_.size() <= key
                || !keyToIndex_.at(key))
            {
                return nullptr;
            }

            at(key) = value;

            return &at(key);
        }

        Type const& insert(
            size_t key,
            Type const& value = Type{})
        {
            // if ( keyToIndex_.size() > key
            //      && keyToIndex_.at( key ) )
            if (contains(key))
            {
                return at(key);
            }

            //* Get new list index for value before modifying values_
            size_t valueIndex = values_.size();

            //* Check size
            keyToIndex_.resize(key + 1, 0);

            //* Add key to valueIndex mapping
            keyToIndex_[key] = valueIndex;

            //* Add valueIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.push_back(key);

            //* Add new value to list
            values_.push_back(value);

            return at(key);
        }

        Type const& insert(
            Type const& value = Type{})
        {
            return insert(
                indexToKey_,
                value);
        }

        Type const& insert_or_assign(
            size_t key,
            Type const& value = Type{})
        {
            // if ( keyToIndex_.size() <= key
            //      || !keyToIndex_.at( key ) )
            if (!contains(key))
            {
                return insert(key, value);
            }

            return *assign(key, value);
        }

        template <typename... Args>
        Type const& emplace(
            size_t key,
            Args&&... args)
        {
            Type value{std::forward<Args>(args)...};

            return insert(key, value);
        }

        template <typename... Args>
        Type const& emplace_or_assign(
            size_t key,
            Args&&... args)
        {
            Type value{std::forward<Args>(args)...};

            // if ( keyToIndex_.size() <= key
            //      || !keyToIndex_.at( key ) )
            if (!contains(key))
            {
                return insert(key, value);
            }

            return *assign(key, value);
        }

        //* Moves last value to gap
        void erase(size_t keyToRemove)
        {
            // if ( keyToRemove > ( keyToIndex_.size() - 1 )
            if (!contains(keyToRemove))
            {
                return;
            }

            size_t size{this->size()};

            //* Replace removed value with last value before popping (if more than one value exists) to keep values contiguous without rearranging the whole vector
            if (size > 1)
            {
                //* Get list index of removed value
                size_t replacedIndex{keyToIndex_[keyToRemove]};

                //* Get index of (kept) last value that replaces removed value
                size_t keptIndex{size - 1};

                //* Get key of replacing/kept value
                size_t keptkey = indexToKey_[keptIndex];

                //* Replace (removed) value with kept value (by index) so last entry (duplicate) can be popped
                values_[replacedIndex] = values_[keptIndex];

                //* Update key to valueIndex mapping for kept key
                keyToIndex_[keptkey] = replacedIndex;

                //* Update index to key mapping for kept key
                indexToKey_[replacedIndex] = keptkey;
            }

            //* Reset removed key in mapping
            keyToIndex_[keyToRemove] = 0;

            //* Remove (duplicate) last value
            values_.pop_back();

            //* Remove removed value from mapping
            indexToKey_.pop_back();
        }

        //* Empty all containers
        void clear()
        {
            values_.clear();
            keyToIndex_.clear();
            indexToKey_.clear();
        }

        //* LOOKUP
        //* Access
        Type const& at(size_t key) const
        {
            return values_.at(keyToIndex_.at(key));
        }

        //* Allow non-const call
        Type& at(size_t key)
        {
            return const_cast<Type&>(std::as_const(*this).at(key));
        }

        //* Access or insert
        Type& operator[](size_t key)
        {
            if (!contains(key))
            {
                insert(key);
            }

            return at(key);
        }

        bool contains(size_t key) const
        {
            if (keyToIndex_.size() <= key)
            {
                return false;
            }

            return keyToIndex_[key];
        }

        size_t find(Type const& value) const
        {
            for (size_t idx{0}; idx < values_.size(); ++idx)
            {
                if (value == values_.at(idx))
                {
                    return indexToKey_[idx];
                }
            }

            return 0;
        }

        //* Get key for value index
        size_t key(size_t index) const
        {
            return indexToKey_.at(index);
        }

        //* Return vector (contiguous memory)
        std::vector<Type> const& values() const
        {
            return values_;
        }

        //* Allow non-const call
        std::vector<Type>& values()
        {
            return const_cast<std::vector<Type>&>(std::as_const(*this).values());
        }

        std::vector<size_t> const& keys() const
        {
            return indexToKey_;
        }

    private:
        //* Vector index is used as value key
        std::vector<Type> values_{Type{}};

        //* Key is used to identify value
        std::vector<size_t> keyToIndex_{};

        //* Store a index (value) to key mapping (internal use for erease() only)
        std::vector<size_t> indexToKey_{0};
    };
}

#endif
