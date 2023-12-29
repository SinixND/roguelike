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
        void insert(Key key, Type element)
        {

            if (test(key))
            {
                return;
            }

            // get new list index for element
            Index elementIndex = elements_.size();

            // add new element to list
            elements_.push_back(element);

            // add key to elementIndex mapping
            keyToIndex_.insert(std::make_pair(key, elementIndex));

            // add elementIndex to key mapping (internal use only to keep list contiguous)
            indexToKey_.insert(std::make_pair(elementIndex, key));
        };

        void erase(Key key)
        {
            if (test(key))
            {
                return;
            };

            // get list index of removed element
            Index removedelementIndex = keyToIndex_[key];

            // replace with last element before popping (if more than one element exists) to keep elements contiguous
            if (elements_.size() > 1)
            {
                // get list index of (kept) last element that replaces removed element
                Index lastelementIndex = elements_.size() - 1;

                // get key of replacing/kept element
                Key keptkey = indexToKey_[lastelementIndex];

                // replace (removed) element with kept element (by index) so last entry (duplicate) can be popped
                elements_[removedelementIndex] = elements_[lastelementIndex];

                // update key to elementIndex mapping for kept key
                keyToIndex_[keptkey] = removedelementIndex;
            }

            // remove removed key from mapping
            keyToIndex_.erase(key);

            // remove removed element from mapping
            indexToKey_.erase(removedelementIndex);

            // remove (duplicate) last element
            elements_.pop_back();
        };

        bool test(Key key)
        {
            return keyToIndex_.count(key);
        };

        Type* get(Key key)
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
        std::vector<Type> elements_{};                // vector index is used as element key
        std::unordered_map<Key, Index> keyToIndex_{}; // key is used to keyentify element
        std::unordered_map<Index, Key> indexToKey_{}; // store a element to key mapping (internal use only)
    };
}

#endif
