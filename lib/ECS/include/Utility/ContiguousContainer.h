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
    template <typename Id, typename Type>
    class ContiguousContainer
    // ContiguousContainer to store elements contiguously in a std::vector but accessible by Id
    {
    public:
        bool tryElement(Id id)
        {
            return id_to_element_.count(id);
        };

        void addElement(Id id, Type element)
        {
            if (tryElement(id))
            {
                return;
            };

            // get new list index for element
            Index elementIndex = elements_.size();

            // add new element to list
            elements_.push_back(element);

            // add id to elementIndex mapping
            id_to_element_.insert(std::make_pair(id, elementIndex));

            // add elementIndex to id mapping (internal use only to keep list contiguous)
            element_to_id_.insert(std::make_pair(elementIndex, id));
        };

        Type& retrieveElement(Id id)
        {

            assert(tryElement(id) && "ELEMENT DOES NOT EXITS!");

            return elements_[id_to_element_[id]];
        };

        void removeElement(Id id)
        {
            if (tryElement(id))
            {
                return;
            };

            // get list index of removed element
            Index removedelementIndex = id_to_element_[id];

            // replace with last element before popping (if more than one element exists) to keep elements contiguous
            if (elements_.size() > 1)
            {
                // get list index of (kept) last element that replaces removed element
                Index lastelementIndex = elements_.size() - 1;

                // get id of replacing/kept element
                Id keptid = element_to_id_[lastelementIndex];

                // replace (removed) element with kept element (by index) so last entry (duplicate) can be popped
                elements_[removedelementIndex] = elements_[lastelementIndex];

                // update id to elementIndex mapping for kept id
                id_to_element_[keptid] = removedelementIndex;
            }

            // remove removed id from mapping
            id_to_element_.erase(id);

            // remove removed element from mapping
            element_to_id_.erase(removedelementIndex);

            // remove (duplicate) last element
            elements_.pop_back();
        };

        std::vector<Type>& retrieveAllElements()
        {
            return elements_;
        };

    private:
        std::vector<Type> elements_;                  // vector index is used as element id
        std::unordered_map<Id, Index> id_to_element_; // id is used to identify element
        std::unordered_map<Index, Id> element_to_id_; // store a element to id mapping for removing
    };
}

#endif