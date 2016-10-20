// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Delegate.hpp"
#include "../misc/SortedVector.hpp"

#include <vector>
#include <type_traits>

namespace util
{
namespace detail
{
    template <typename Return, typename... Params> class EventBase
    {
    public:
        typedef util::Delegate<Return(Params...)> Delegate;
        typedef sorted_vector<Delegate>           Container;

        inline typename Container::size_type size() const { return delegates.size(); }
        inline bool empty() const { return delegates.empty(); }

        inline void clear() { delegates.clear(); }

        void operator() (Params... params) const
        {
            for (auto const& d : delegates)
                d(params...);
        }

        inline bool operator == (EventBase const& e) { return delegates == e.delegates; }
        inline bool operator != (EventBase const& e) { return delegates != e.delegates; }
        inline bool operator <  (EventBase const& e) { return delegates <  e.delegates; }
        inline bool operator <= (EventBase const& e) { return delegates <= e.delegates; }
        inline bool operator >  (EventBase const& e) { return delegates >  e.delegates; }
        inline bool operator >= (EventBase const& e) { return delegates >= e.delegates; }

    protected:
        Container delegates;

        inline bool addDelegate(Delegate const& d)
        {
            return delegates.insert(d).second;
        }

        inline bool removeDelegate(Delegate const& d)
        {
            return delegates.erase(d) > 0;
        }
    };

    template <bool HasEvents, typename Return, typename... Params> class EventB : public detail::EventBase<Return,Params...>
    {
    public:
        using detail::EventBase<Return,Params...>::addDelegate;
        using detail::EventBase<Return,Params...>::removeDelegate;
        using Delegate = typename detail::EventBase<Return,Params...>::Delegate;

        EventB<false, void, Delegate const&> onAdd;
        EventB<false, void, Delegate const&> onRemove;

        template <typename... P> inline void add(P&&... params)
        {
            Delegate const d = Delegate(std::forward<P>(params)...);
            if (addDelegate(d)) onAdd(d);
        }

        template <typename... P> inline void remove(P&&... params)
        {
            Delegate const d = Delegate(std::forward<P>(params)...);
            if (removeDelegate(d)) onRemove(d);
        }
    };

    template<typename Return, typename... Params> class EventB<false, Return, Params...> : public detail::EventBase<Return, Params...>
    {
    public:
        using detail::EventBase<Return,Params...>::addDelegate;
        using detail::EventBase<Return,Params...>::removeDelegate;
        using Delegate = typename detail::EventBase<Return,Params...>::Delegate;

        template <typename... Args> inline void add(Args&&... args)
        {
            addDelegate(Delegate(std::forward<Args>(args)...));
        }

        template <typename... Args> inline void remove(Args&&... args)
        {
            removeDelegate(Delegate(std::forward<Args>(args)...));
        }
    };
}

    template <typename Return, typename... Params> class Event;
    template <typename Return, typename... Params> class Event<Return(Params...)> : public detail::EventB<true, Return, Params...>
    {};
}
