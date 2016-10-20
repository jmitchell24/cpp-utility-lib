// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Delegate.hpp"

#include <vector>

namespace util
{
    template <typename Return, typename... Params> class EventOnce;
    template <typename Return, typename... Params> class EventOnce<Return(Params...)>
    {
    public:
        typedef util::Delegate<Return(Params...)> Delegate;

        template <typename... Args> inline void add(Args... args)
        {
            delegates.push_back(Delegate(std::forward<Args>(args)...));
        }

        inline void clear()
        {
            delegates.clear();
        }

        inline void operator() (Params... params)
        {
            if (!delegates.empty())
            {
                for (auto const& d : delegates)
                    d(params...);
                delegates.clear();
            }
        }

    private:
        std::vector<Delegate> delegates;
    };
}
