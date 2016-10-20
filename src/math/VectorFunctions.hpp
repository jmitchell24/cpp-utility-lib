// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

namespace util
{
    template <typename V> inline auto sum       (V const& v) -> decltype(v.sum())        { return v.sum(); }
    template <typename V> inline auto sumSquared(V const& v) -> decltype(v.sumSquared()) { return v.sumSquared(); }
    template <typename V> inline auto length    (V const& v) -> decltype(v.length())     { return v.length(); }

    template <typename V> inline auto dot     (V const& a, V const& b) -> decltype(a.dot(b))      { return a.dot(b); }
    template <typename V> inline auto angle   (V const& a, V const& b) -> decltype(a.angle(b))    { return a.angle(b); }
    template <typename V> inline auto distance(V const& a, V const& b) -> decltype(a.distance(b)) { return a.distance(b); }

    template <typename V> inline auto floor(V const& v) -> decltype(v.floor()) { return v.floor(); }
    template <typename V> inline auto ceil (V const& v) -> decltype(v.ceil())  { return v.ceil(); }
    template <typename V> inline auto round(V const& v) -> decltype(v.round()) { return v.round(); }

    template <typename V> inline auto floor(V const& v, typename V::NumParam n) -> decltype(v.floor(n)) { return v.floor(n); }
    template <typename V> inline auto ceil (V const& v, typename V::NumParam n) -> decltype(v.ceil(n))  { return v.ceil(n); }
    template <typename V> inline auto round(V const& v, typename V::NumParam n) -> decltype(v.round(n)) { return v.round(n); }

    template <typename V> inline auto reverse(V const& v) -> decltype(v.reverse()) { return v.reverse(); }
    template <typename V> inline auto normal (V const& v) -> decltype(v.normal())  { return v.normal(); }

    template <typename V> inline auto project(V const& v, V const& base)    -> decltype(v.project(base)) { return v.project(base); }
    template <typename V> inline auto reflect(V const& v, V const& normal)  -> decltype(v.reflect(normal)) { return v.reflect(normal); }

    template <typename V> inline bool isNan(V const& v) { return v.isNan(); }
    template <typename V> inline bool isInf(V const& v) { return v.isInf(); }
}
