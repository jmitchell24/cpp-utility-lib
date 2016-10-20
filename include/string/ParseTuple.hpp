#pragma once

#include "Types.hpp"
#include "Parse.hpp"
#include "../misc/ParameterPack.hpp"

#include <tuple>

namespace util
{
    template <typename C, typename... Params> bool basicTryParseTuple(SegmentT<C> const& s, Params&... params);
    template <typename C, typename... Params> bool basicTryParseTuple(std::basic_istream<C>& is, Params&... params);

    template <typename... Params> bool tryParseTuple(Segment const& s, Params&... params);
    template <typename... Params> bool tryParseTuple(istream_t& is, Params&... params);

namespace detail
{
    template <typename C, typename Param> inline static bool basicTryParseTupleImpl(std::basic_istream<C>& is, Param& param)
    {
        C close;
        return util::basicTryParse(is,param) && (is >> close) && close == UTIL_DEFAULT_UNNEST;
    }

    template <typename C, typename Param,  typename... Params> inline static bool basicTryParseTupleImpl(std::basic_istream<C>& is, Param& param, Params&... params)
    {
        C delim;
        return util::basicTryParse(is,param) && (is >> delim) && delim == UTIL_DEFAULT_DELIM && basicTryParseTupleImpl<C>(is,params...);
    }
}
}

template <typename C, typename... Params> bool util::basicTryParseTuple(std::basic_istream<C>& is, Params&... params)
{
    C open;
    if (!(is >> open) || open != UTIL_DEFAULT_NEST)
        return false;
    typename varargs::TupleHelper<Params...>::Tuple tuple(params...);
    if (!varargs::TupleHelper<Params...>::call(tuple, [&](Params&... params) -> bool
    { return util::detail::basicTryParseTupleImpl<C,Params...>(is, params...); }))
        return false;
    varargs::TupleHelper<Params...>::set(tuple, params...);
    return true;
}

template <typename C, typename... Params> bool util::basicTryParseTuple(SegmentT<C> const& s, Params&... params)
{
    typename SegmentT<C>::StreamBuffer buf(s);
    std::basic_istream<C> is(&buf);
    return basicTryParseTuple<C,Params...>(is, params...);
}

template <typename... Params> bool util::tryParseTuple(istream_t& is, Params&... params)
{
    return basicTryParseTuple(is, params...);
}

template <typename... Params> bool util::tryParseTuple(Segment const& s, Params&... params)
{
    return basicTryParseTuple(s, params...);
}
