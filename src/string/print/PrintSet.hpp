// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "PrintAlignment.hpp"
#include "PrintParameter.hpp"
#include "../../typetraits/HasOstreamOut.hpp"

#include <iomanip>
#include <type_traits>

namespace util
{
namespace detail
{
    template<typename A, typename B>
    static typename std::enable_if<std::is_convertible<B, A>::value, void>::type set(A& a, B&& b)
    { a = A(std::move(b)); }

    template<typename A, typename B>
    static typename std::enable_if<!std::is_convertible<B, A>::value, void>::type set(A&, B&&)
    {}

    template <typename B>
    static typename std::enable_if<std::is_integral<B>::value, void>::type set(Alignment& a, B&& b)
    { a = Alignment(std::move(b)); }

    template <typename B>
    static typename std::enable_if<has_insertion_operator<B>::value, void>::type set(Parameter& p, B&& b)
    {
        ostringstream_t stream;

        if (p.transform != nullptr && p.alignment != nullptr)
            p.alignment->applyAndTransform(stream, *p.transform, std::move(b));
        else if (p.alignment != nullptr)
            p.alignment->apply(stream, std::move(b));
        else if (p.transform != nullptr)
            transformToStream(stream, *p.transform, std::move(b));
        else
            stream << std::move(b);

        p.value->assign(stream.str());
        *p.token = Segment(*p.value);
    }
}
}
