#ifndef CONTIGUOUSMAP_H_20231231160415
#define CONTIGUOUSMAP_H_20231231160415

#include <unordered_map>
#include <vector>

typedef size_t Index;

namespace snd
{
    class BaseContiguousMap
    {
    public:
        virtual ~BaseContiguousMap() = default;
    };

    template <typename Key, typename Type>
    class ContiguousMap : public BaseContiguousMap
    {
    public:
        void insert(const Key& key, const Type& element)
        {

            if (test(key))
            {
                return;
            }

            // Get new list index for element
            Index elementIndex = elements_.size();

            // Add new element to list
            elements_.push_back(element);

            // Add key to elementIndex mapping
            keyToIndex_.insert(std::make_pair(key, elementIndex));

            // Add elementIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.insert(std::make_pair(elementIndex, key));
        };

        void erase(const Key& key)
        {
            if (test(key))
            {
                return;
            };

            // Get list index of removed element
            Index removedelementIndex = keyToIndex_[key];

            // Replace with last element before popping (if more than one element exists) to keep elements contiguous
            if (elements_.size() > 1)
            {
                // Get list index of (kept) last element that replaces removed element
                Index lastelementIndex = elements_.size() - 1;

                // Get key of replacing/kept element
                Key keptkey = indexToKey_[lastelementIndex];

                // Replace (removed) element with kept element (by index) so last entry (duplicate) can be popped
                elements_[removedelementIndex] = elements_[lastelementIndex];

                // Update key to elementIndex mapping for kept key
                keyToIndex_[keptkey] = removedelementIndex;
            }

            // Remove removed key from mapping
            keyToIndex_.erase(key);

            // Remove removed element from mapping
            indexToKey_.erase(removedelementIndex);

            // Remove (duplicate) last element
            elements_.pop_back();
        };

        bool test(const Key& key)
        {
            return keyToIndex_.count(key);
        };

        Type* get(const Key& key)
        {

            if (!test(key))
                return nullptr;

            return &elements_[keyToIndex_[key]];
        };

        std::vector<Type>* getAll()
        {
            return elements_;
        };

    private:
        std::vector<Type> elements_{};                // Vector index is used as element key
        std::unordered_map<Key, Index> keyToIndex_{}; // Key is used to identify element
        std::unordered_map<Index, Key> indexToKey_{}; // Store a index (element) to key mapping (internal use only)
    };
}

#endif
