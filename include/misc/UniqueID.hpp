#pragma once

#include "../typetraits/CallTraits.hpp"

#include <vector>

namespace util
{
    template <typename Id> class UniqueID
    {
    public:
        typedef best_param<Id> IdParam;

        UniqueID()
            : counter(Id()), available()
        { }

        Id aquire()
        {
            if (available.empty())
                return counter++;
            Id const id = available.back();
            available.pop_back();
            return id;
        }

        void release(IdParam id)
        {
            available.emplace_back(id);
        }

    private:
        Id counter;
        std::vector<Id> available;
    };
}
