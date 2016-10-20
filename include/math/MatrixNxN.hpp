// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "../Types.hpp"
#include "../String.hpp"
#include "../string/Segment.hpp"
#include "../Assertion.hpp"
#include "../typetraits/CallTraits.hpp"

#include "VectorND.hpp"

namespace util
{
    template <typename N, size_t R, size_t C> struct MatNRC
    {
        typedef N                 Num;
        typedef Num&              NumRef;
        typedef best_param<Num>   NumParam;

        typedef VecND<N,C>        Vec;
        typedef Vec&              VecRef;
        typedef best_param<Vec>   VecParam;

        typedef MatNRC<Num,R,C>   Mat;
        typedef Mat&              MatRef;
        typedef best_param<Mat>   MatParam;

        typedef N                 Components[R*C];

        size_t static constexpr Rows    = R;
        size_t static constexpr Columns = C;
        size_t static constexpr Size    = Rows*Columns;

        Components components;

        inline MatNRC()
        {
            std::fill(begin(), end(), (Num)0);
        }

        inline explicit MatNRC(Mat&& v)
            : components(std::move(v.components))
        {}

        template <typename P>
        inline explicit MatNRC(MatNRC<P,R,C> const& m)
            : MatNRC(m.begin(), m.end())
        {}

        inline explicit MatNRC(NumParam n)
        {
            std::fill(begin(), end(), n);
        }

        inline explicit MatNRC(Components&& components)
            : components(std::move(components))
        {}

        inline explicit MatNRC(Components const& components)
            : components(components)
        {}

        template <typename P>
        inline MatNRC(std::initializer_list<P> const& il)
            : MatNRC(il.begin(), il.end())
        {}

        template <typename I>
        inline MatNRC(I first, I last)
            : MatNRC(make_matrix<N,R,C>(first, last))
        {}

        inline bool operator== (MatParam m) const { return std::equal(begin(), end(), m.begin()); }
        inline bool operator!= (MatParam m) const { return !(*this == m); }
        inline bool operator<  (MatParam m) const { return std::lexicographical_compare(begin(), end(), m.begin(), m.end()); }
        inline bool operator>  (MatParam m) const { return *this < m; }
        inline bool operator<= (MatParam m) const { return !(*this < m); }
        inline bool operator>= (MatParam m) const { return !(m < *this); }

        inline NumParam operator[] (size_t const i) const { ASSERT(i < Size); return components[i]; }
        inline Num    & operator[] (size_t const i)       { ASSERT(i < Size); return components[i]; }

        inline Num*       begin()       { return std::begin(components); }
        inline Num const* begin() const { return std::begin(components); }

        inline Num*       end()       { return std::end(components); }
        inline Num const* end() const { return std::end(components); }

        inline string_t str() const
        {
            ostringstream_t oss;
            for (size_t i = 0; i < R-1; ++i)
                oss << *(reinterpret_cast<Vec const*>(&components[i*R])) << std::endl;
            return oss.str();
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Mat)

        static_assert(R > 1, "R must be greater than one");
        static_assert(C > 1, "C must be greater than one");
    };

    template <typename N, size_t R, size_t C, typename It>
    inline MatNRC<N,R,C> make_matrix(It first, It last)
    {
        MatNRC<N,R,C> m;
        auto const e = m.begin();
        auto       c = m.end();
        for (auto i = first; i != last && c != e; ++i, ++c)
            *c = (N)*i;
        return m;
    }
}
