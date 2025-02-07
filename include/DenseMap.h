#ifndef IG20240128195657
#define IG20240128195657

#include <cassert>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace snx
{
    template <typename Key, typename Type>
    class DenseMap
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
        //* Return size of DenseMap
        size_t size() const
        {
            return values_.size();
        }

        //* MODIFIERS
        Type& assign(
            Key const& key,
            Type const& value = Type{}
        )
        {
            ( *this )[key] = value;

            return ( *this )[key];
        }

        Type& insert(
            Key const& key,
            Type const& value = Type{}
        )
        {
            if ( keyToIndex_.contains( key ) )
            {
                return ( *this )[key];
            }

            //* Get new list index for value before modifying values_
            size_t valueIndex = size();

            //* Add key to valueIndex mapping
            keyToIndex_.insert( { key, valueIndex } );

            //* Add valueIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.push_back( key );

            //* Add new value to list
            values_.push_back( value );

            assert( ( keyToIndex_.size() == indexToKey_.size() ) && "Internal mismatch!" );

            return ( *this )[key];
        }

        Type& insert_or_assign(
            Key const& key,
            Type const& value = Type{}
        )
        {
            if ( !keyToIndex_.contains( key ) )
            {
                return insert( key, value );
            }

            return assign( key, value );
        }

        template <typename... Args>
        Type& emplace(
            Key const& key,
            Args&&... args
        )
        {
            Type value{ std::forward<Args>( args )... };

            return insert( key, value );
        }

        template <typename... Args>
        Type& emplace_or_assign(
            Key const& key,
            Args&&... args
        )
        {
            Type value{ std::forward<Args>( args )... };

            if ( !keyToIndex_.contains( key ) )
            {
                return insert( key, value );
            }

            return assign( key, value );
        }

        //* Moves last value to gap
        void erase( Key const& key )
        {
            if ( !keyToIndex_.contains( key ) )
            {
                return;
            }

            //* Get list index of removed value
            size_t removedValueIndex{ keyToIndex_[key] };
            size_t keptValueIndex{};
            size_t size{ this->size() };

            //* Replace removed value with last value before popping (if more than one value exists) to keep values contiguous
            if ( size > 1 )
            {
                //* Get index of (kept) last value that replaces removed value
                keptValueIndex = size - 1;

                //* Get key of replacing/kept value
                Key keptkey = indexToKey_[keptValueIndex];

                //* Replace (removed) value with kept value (by index) so last entry (duplicate) can be popped (becomes new removed)
                values_[removedValueIndex] = values_[keptValueIndex];

                //* Update value index after replacement
                keptValueIndex = removedValueIndex;

                //* Update key to valueIndex mapping for kept key
                keyToIndex_[keptkey] = keptValueIndex;

                //* Update index to key mapping for kept key
                indexToKey_[keptValueIndex] = keptkey;
            }

            //* Remove removed key from mapping
            keyToIndex_.erase( key );

            //* Remove (duplicate) last value
            values_.pop_back();

            //* Remove removed value from mapping
            indexToKey_.pop_back();
        }

        void changeKey(
            Key const& from,
            Key const& to
        )
        {
            assert( !contains( to ) && "Key already exists, possible loss of data!" );

            //* Add new key with old index
            keyToIndex_[to] = index( from );

            //* Remove old key
            keyToIndex_.erase( from );

            //* Remap index to new key
            indexToKey_[index( to )] = to;
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
        Type const& operator[]( Key const& key ) const
        {
            return values_[index( key )];
        }

        //* Allow non-const calls
        Type& operator[]( Key const& key )
        {
            return const_cast<Type&>( std::as_const( *this )[key] );
        }

        //* Access or insert
        // Type& operator[]( Key const& key )
        // {
        //     if ( !contains( key ) )
        //     {
        //         insert( key );
        //     }
        //
        //     return at( key );
        // }

        bool contains( Key const& key ) const
        {
            return keyToIndex_.contains( key );
        }

        //* Get key for value index
        Key const& key( size_t index ) const
        {
            return indexToKey_[index];
        }

        //* Get value index for key
        size_t const& index( Key const& key ) const
        {
            return keyToIndex_.at( key );
        }

        //* Return vector (contiguous memory)
        std::vector<Type> const& values() const
        {
            return values_;
        }

        //* Allow non-const call
        std::vector<Type>& values()
        {
            return const_cast<std::vector<Type>&>( std::as_const( *this ).values() );
        }

    private:
        Type const& at( Key const& key ) const
        {
            return values_[index( key )];
        }

        Type& at( Key const& key )
        {
            return const_cast<Type&>( std::as_const( *this ).at( key ) );
        }

    private:
        //* Vector index is used as value key
        std::vector<Type> values_{};

        //* Key is used to identify value
        std::unordered_map<Key, size_t> keyToIndex_{};

        //* Store a index (value) to key mapping
        std::vector<Key> indexToKey_{};
    };
}

#endif
