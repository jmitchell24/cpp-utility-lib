// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Delegate.hpp"
#include "../Assertion.hpp"

#include <algorithm>
#include <vector>
#include <map>

namespace util
{
    template <typename Compare, typename Return, typename... Params> class EventSorted;
    template <typename Compare, typename Return, typename... Params> class EventSorted<Compare, Return(Params...)>
	{
    public:
        using Delegate = util::Delegate<Return(Params...)>;

        detail::EventB<false, void, Delegate const&> onAdd;
        detail::EventB<false, void, Delegate const&> onRemove;

        template <typename... Args> inline void add(Compare const& compare, Args... args)
        {
            addDelegate(compare, Delegate(std::forward<Args>(args)...));
        }

        template <typename... Args> inline void remove(Args... args)
        {
            removeDelegate(Delegate(std::forward<Args>(args)...));
        }

        inline void clear()
        {
            delegates.clear();
        }

        inline void operator() (Params... params) const
        {
            for (auto const& d : delegates)
                d(params...);
        }

    private:
        std::vector<Delegate> delegates;
        std::map<Delegate, Compare> delegates_compare;

        void addDelegate(Compare const& compare, Delegate const& d)
		{
            if (std::find(delegates.begin(),delegates.end(),d) == delegates.end())
            {
                delegates_compare[d] = compare;
                delegates.push_back(d);
                onAdd(d);

                std::sort(delegates.begin(), delegates.end(), [this](Delegate const& a, Delegate const& b)
                {
                    ASSERT(delegates_compare.find(a) != delegates_compare.end());
                    return delegates_compare[a] < delegates_compare[b];
                });
            }
        }

        void removeDelegate(Delegate const& d)
		{
            auto const i = std::find(delegates.begin(),delegates.end(),d);
            if (i != delegates.end())
            {
                delegates.erase(i);
                delegates_compare.erase(d);
                onRemove(d);
            }
		}
	};
}
