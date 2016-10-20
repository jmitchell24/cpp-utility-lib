// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Delegate.hpp"
#include "../Exception.hpp"

namespace util
{
    template <typename Return, typename... Params> class Signal;
    template <typename Return, typename... Params> class Signal<Return(Params...)>
    {
    public:
        typedef util::Delegate<Return(Params...)> Delegate;

        inline Signal()
            : delegate()
        {}

        template<typename... Args>
        inline Signal(Args... args)
            : delegate(std::forward<Args>(args)...)
        {}

        inline Signal(Signal<Return,Params...> const& signal)
            : delegate(signal.delegate)
		{}

        template<typename... Args>
        inline void set(Args... args)
        {
            delegate = Delegate(std::forward<Args>(args)...);
        }

        inline void clear()
        {
            delegate.clear();
        }

        inline bool empty() const
        {
            return delegate.empty();
        }

        inline Return operator() (Params... params) const
        {
            if (!delegate)
                throw InvalidSignalException();
            return delegate(params...);
        }

    private:
        Delegate delegate;
    };
}
