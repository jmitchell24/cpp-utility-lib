// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "PrintSet.hpp"
#include "../../misc/StaticBuffer.hpp"

namespace util
{
namespace detail
{
    template <typename Type> class Part
    {
        struct Pair
        {
            unsigned index;
            Type*    t;

            inline Pair(unsigned index, Type* t)
                : index(index), t(t)
            {}

            inline bool operator< (Pair const& p) const
            { return index < p.index; }
        };

        //using Index     = std::vector<Pair>; // better performance in some circumstances ???
        using Index     = StaticBuffer<Pair,PART_LIMIT>;
        using IndexIt   = typename Index::iterator;
        using IndexPair = typename Index::value_type;
        using Buffer    = StaticBuffer<Type,PART_LIMIT>;

        template<unsigned I, typename T>
        inline static void setPart(IndexIt& i, IndexIt const& ie, T&& t)
        {
            for (; i->index == I && i != ie; ++i)
                set(*i->t, t);
        }

        template<unsigned I, typename T>
        inline static void setParts(IndexIt i, IndexIt const& ie, T&& t)
        { setPart<I,T>(i, ie, t); }

        template<unsigned I, typename T, typename... Params>
        inline static void setParts(IndexIt i, IndexIt const& ie, T&& t, Params&&... params)
        {
            if (i != ie)
            {
                setPart<I,T>(i, ie, t);
                setParts<I+1,Params...>(i,ie,params...);
            }
        }

    public:
        Index  index;
        Buffer buffer;

        inline Type* addKnown(util::Segment const& s)
        {
            Type value;
            if (!tryParse(s, value)) return nullptr;
            buffer.emplace_back(value);
            return &buffer.back();
        }

        inline Type* addUnknown(unsigned index)
        {
            buffer.emplace_back();
            this->index.emplace_back(index, &buffer.back());
            return &buffer.back();
        }

        inline Type* addUnknown(unsigned index, Type const& value)
        {
            buffer.emplace_back(value);
            this->index.emplace_back(index,  &buffer.back());
            return &buffer.back();
        }

        template <typename... Params> inline void params(Params&&... params)
        {
            if (!index.empty())
            {
                std::sort(index.begin(), index.end());
                setParts<0>(index.begin(), index.end(), params...);
            }
        }
    };
}
}
