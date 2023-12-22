#ifndef CONTIGUOUSCONTAINER_H_20231216163005
#define CONTIGUOUSCONTAINER_H_20231216163005

#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

typedef size_t Index;

namespace snd
{
    template <typename Key, typename Type>
    class ContiguousContainer
    // ContiguousContainer to store elements contiguously in a std::vector but accessible by key
    {
    public:
        bool tryElement(Key key)
        {
            return key_to_element_.count(key);
        };

        void addElement(Key key, Type element)
        {

            if (tryElement(key))
            {
                return;
            }

            // get new list index for element
            Index elementIndex = elements_.size();

            // add new element to list
            elements_.push_back(element);

            // add key to elementIndex mapping
            key_to_element_.insert(std::make_pair(key, elementIndex));

            // add elementIndex to key mapping (internal use only to keep list contiguous)
            element_to_key_.insert(std::make_pair(elementIndex, key));
        };

        Type* retrieveElement(Key key)
        {

            if (!tryElement(key))
                return nullptr;

            return &elements_[key_to_element_[key]];
        };

        void removeElement(Key key)
        {
            if (tryElement(key))
            {
                return;
            };

            // get list index of removed element
            Index removedelementIndex = key_to_element_[key];

            // replace with last element before popping (if more than one element exists) to keep elements contiguous
            if (elements_.size() > 1)
            {
                // get list index of (kept) last element that replaces removed element
                Index lastelementIndex = elements_.size() - 1;

                // get key of replacing/kept element
                Key keptkey = element_to_key_[lastelementIndex];

                // replace (removed) element with kept element (by index) so last entry (duplicate) can be popped
                elements_[removedelementIndex] = elements_[lastelementIndex];

                // update key to elementIndex mapping for kept key
                key_to_element_[keptkey] = removedelementIndex;
            }

            // remove removed key from mapping
            key_to_element_.erase(key);

            // remove removed element from mapping
            element_to_key_.erase(removedelementIndex);

            // remove (duplicate) last element
            elements_.pop_back();
        };

        std::vector<Type>& retrieveAllElements()
        {
            return elements_;
        };

    private:
        std::vector<Type> elements_{};                  // vector index is used as element key
        std::unordered_map<Key, Index> key_to_element_{}; // key is used to keyentify element
        std::unordered_map<Index, Key> element_to_key_{}; // store a element to key mapping for removing
    };
}

#endif
