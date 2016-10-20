// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "../string/Types.hpp"





namespace util
{
namespace has_insertion_operator_impl
{
    typedef char no;
    typedef char yes[2];

    struct any_t { template<typename T> any_t(T const&); };

    no operator<<(util::ostream_t const&, any_t const&);

    yes& test(util::ostream_t&);
    no test( no );

    template<typename T> struct has_insertion_operator
    {
        static util::ostream_t& s;
        static T const& t;
        static bool const value = sizeof( test(s << t) ) == sizeof( yes );
    };
}

    template<typename T> struct has_insertion_operator :
    has_insertion_operator_impl::has_insertion_operator<T> {};

namespace has_extraction_operator_impl
{
    typedef char no;
    typedef char yes[2];

    struct any_t { template<typename T> any_t(T const&); };

    no operator>>(util::istream_t const&, any_t const&);

    yes& test( util::istream_t& );
    no test( no );

    template<typename T> struct has_extraction_operator
    {
        static util::istream_t& s;
        static T const& t;
        static bool const value = sizeof( test(s >> t) ) == sizeof( yes );
    };
}

    template<typename T> struct has_extraction_operator :
    has_extraction_operator_impl::has_extraction_operator<T> {};
}
