// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "../Assertion.hpp"

namespace util
{
    template <typename C> class SegmentT
    {
    public:    
        struct StreamBuffer : public std::basic_streambuf<C>
        {
            StreamBuffer(SegmentT<C> const& s)
            {
                Char* const begin = const_cast<Char*>(s.begin);
                Char* const end   = const_cast<Char*>(s.end);
                std::basic_streambuf<C>::setg(begin,begin,end);
            }
        };

        typedef C                       Char;
        typedef C const                 CharParam;
        typedef std::char_traits<C>     CharTraits;
        typedef SegmentT<Char>          Segment;
        typedef Segment const&          SegmentParam;
        typedef std::basic_string<Char> StlString;
        typedef StlString const&        StlStringParam;

        typedef size_t      Size;
        typedef Char const* Location;

        union
        {
            Char const* const begin;
            Char const* const ptr;
        };

        Char const* const end;

        bool const null_terminated;

        SegmentT()
            : begin(&default_segment_location), end(begin), null_terminated(true)
        {}

        SegmentT(Location const s, Size const length)
            : begin(s), end(s+length), null_terminated(*end == '\0')
        {}

        SegmentT(Location const begin, Location const end)
            : begin(begin), end(end), null_terminated(*end == '\0')
        { ASSERT(this->begin <= this->end); }

        SegmentT(Location const begin)
            : begin(begin), end(begin + CharTraits::length(begin)), null_terminated(*end == '\0')
        { ASSERT(this->begin <= this->end); }

        SegmentT(StlStringParam s)
            : begin(&s[0]), end(begin+s.size()), null_terminated(*end == '\0')
        { ASSERT(this->begin <= this->end); }

        inline ~SegmentT()
        {}

        inline Segment sub(Location const begin) const
        {
            return sub(begin, end);
        }

        inline Segment sub(Size const length) const
        {
            return sub(begin, begin+length);
        }

        inline Segment sub(Size const begin, Size const length) const
        {
            return sub(this->begin+begin, this->begin+begin+length);
        }

        inline Segment sub(Location const begin, Size const length) const
        {
            return sub(begin, begin+length);
        }

        inline Segment sub(Location const begin, Location const end) const
        {
            ASSERT(begin >= this->begin);
            ASSERT(begin <= this->end);
            ASSERT(end <= this->end);
            ASSERT(end >= this->begin);
            return Segment(begin,end);
        }

        static bool same(SegmentParam a, SegmentParam b)
        {
            if (a.size() != b.size())
                return false;

            Location ia=a.begin, ib=b.begin;
            for (; ia != a.end; ++ia,++ib)
                if (!CharTraits::eq(*ia,*ib))
                    return false;
            return true;
        }

        static bool same(SegmentParam a, CharParam b)
        {
            if (a.size() != 1)
                return false;
            return CharTraits::eq(*a.begin, b);
        }

        static int compare(SegmentParam a, SegmentParam b)
        {
            return CharTraits::compare(a.begin, b.begin, std::min(a.size(), b.size()));
        }

        static int compare(SegmentParam a, CharParam b)
        {
            return CharTraits::compare(a.begin, &b, 1);
        }

        inline bool same(SegmentParam s) const
        {
            return same(*this,s);
        }

        inline bool same(CharParam c) const
        {
            return same(*this,c);
        }

        inline int compare(SegmentParam s) const
        {
            return compare(*this, s);
        }

        inline int compare(CharParam c) const
        {
            return compare(*this, c);
        }

        Location find(SegmentParam s) const
        {
            Size const size = s.size();
            if (size > this->size())
                return end;

            for(Location i = begin; i != end-s.size()+1; ++i)
                if (sub(i,size).same(s))
                    return i;
            return end;
        }

        inline Location find(CharParam c) const
        {
            return CharTraits::find(ptr, size(), c);
        }

        inline Location findFirst(SegmentParam s) const
        {
            return find(s);
        }

        inline Location findFirst(CharParam c) const
        {
            return find(c);
        }

        Location findFirstNot(CharParam c) const
        {
            for (Location i = begin; i != end; ++i)
                if (*i != c)
                    return i;
            return end;
        }

        Location findLast(SegmentParam s) const
        {
            Size const size = s.size();
            if (size > this->size())
                return end;
            for(Location i = end-s.size(); i != begin-1; --i)
                if (sub(i,size).same(s))
                    return i;
            return end;
        }

        Location findLast(CharParam c) const
        {
            if (begin == end)
                return begin;
            for (Location i = end-1; i != begin-1; --i)
                if (*i == c)
                    return i;
            return end;
        }

        Location findLastNot(CharParam c) const
        {
            for (Location i = end-1; i != begin-1; --i)
                if (*i != c)
                    return i;
            return end;
        }

        inline bool contains(SegmentParam s) const
        {
            return find(s) != end;
        }

        inline bool contains(CharParam c) const
        {
            return find(c) != end;
        }

        inline StlString str() const
        {
            return StlString(begin, end);
        }

        inline Size size() const
        {
            return end - begin;
        }

        inline bool empty() const
        {
            return begin == end;
        }

        inline Segment& operator= (SegmentParam s)
        {
            return *(new (this) Segment(s.begin,s.end));
        }

        inline Char operator[] (Size const i) const
        {
            ASSERT(begin+i >= this->begin);
            ASSERT(begin+i <= this->end);
            return begin[i];
        }

        inline bool operator<  (SegmentParam s) const { return compare(s) <  0; }
        inline bool operator>  (SegmentParam s) const { return compare(s) >  0; }
        inline bool operator== (SegmentParam s) const { return  same(s); }
        inline bool operator!= (SegmentParam s) const { return !same(s); }

        inline friend ostream_t& operator<< (ostream_t& os, SegmentParam s)
        {
            return os << s.str();
            //std::ostream::sentry _s(os);
            //if (_s) os.write(s.begin, s.size());
            //return os;
        }

    private:
        Char static const default_segment_location;
    };

    template<typename C>
    typename SegmentT<C>::Char const SegmentT<C>::default_segment_location = 0;

    template <typename C>
    class StringT : public SegmentT<C>
    {
        template <typename _C, unsigned N>
        friend StringT<_C> literal(_C const(&array)[N]);

        StringT(typename SegmentT<C>::Location begin, typename SegmentT<C>::Location end)
            : SegmentT<C>(begin,end)
        { ASSERT(SegmentT<C>::null_terminated); }

    public:
        template <typename... Params> StringT(Params... params)
            : SegmentT<C>(std::forward<Params>(params)...)
        { ASSERT(SegmentT<C>::null_terminated); }
    };

    template <typename C>
    inline char_t const* begin(SegmentT<C> const& s)
    { return s.begin; }

    template <typename C>
    inline char_t const* end(SegmentT<C> const& s)
    { return s.end; }

    template <typename C, unsigned N>
    inline StringT<C> literal(C const(&array)[N])
    { return StringT<C>(array,array+N-1); }
}
