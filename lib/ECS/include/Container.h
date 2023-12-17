/* DESCRIPTION:
// Container to store elements contiguously but accessible by Id
*/

#ifndef CONTAINER_H_20231216163005
#define CONTAINER_H_20231216163005

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <vector>

typedef size_t Index;

namespace snd
{
    template <typename Id, typename Type>
    class Container
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

            Index elementIndex = elements_.size();

            elements_.push_back(element);
            id_to_element_.insert(std::make_pair(id, elementIndex));
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

            Index removedelementIndex = id_to_element_[id];

            // replace with last element before popping if more than one element to keep elements contiguous
            if (elements_.size() > 1)
            {
                Index lastelementIndex = elements_.size() - 1;
                Id keptid = element_to_id_[lastelementIndex];

                // swap elements (by index) so last entry can be popped
                elements_[removedelementIndex] = elements_[lastelementIndex];
                // update mapping (by id)
                id_to_element_[keptid] = removedelementIndex;
            }

            // remove removed id from mapping
            id_to_element_.erase(id);
            // remove removed element from mapping
            element_to_id_.erase(removedelementIndex);

            elements_.pop_back();
        };

        void iterateAllElements(std::function<void(Type element)> lambda)
        {
            for (auto& element : elements_)
            {
                lambda(element);
            };
        };

    private:
        std::vector<Type> elements_;        // vector index is used as element id
        std::map<Id, Index> id_to_element_; // id is used to identify element
        std::map<Index, Id> element_to_id_; // store a element to id mapping for removing
    };
}

#endif