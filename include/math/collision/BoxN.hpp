// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../Types.hpp"
#include "../../typetraits/CallTraits.hpp"
#include "../../String.hpp"

namespace util
{
    enum OriginDimension { MIN, CENTER, MAX };

    template<typename N, size_t D> struct BoxND
    {
    public:
        typedef N            Num;
        typedef VecND<Num,D> Vec;
        typedef BoxND<Num,D> Box;

        typedef best_param<Num> NumParam;
        typedef best_param<Vec> VecParam;
        typedef best_param<Box> BoxParam;

        size_t static constexpr Size = D;

        union
        {
            struct
            {
                Vec position;
                Vec size;
            };

            N components[sizeof(Vec)*2];
        };

        Vec origin;

        inline constexpr BoxND()
            : position(), size(), origin()
        {}

        inline constexpr BoxND(VecParam position, VecParam size)
            : position(position), size(size), origin()
        {}

        inline constexpr BoxND(VecParam position, VecParam size, VecParam origin)
            : position(position), size(size), origin(origin)
        {}

        inline constexpr BoxND(Box const& box)
            : position(box.position), size(box.size)
        {}

        template <typename P>
        inline explicit constexpr BoxND(BoxND<P, D> const& box)
            : position(box.position), size(box.size), origin(box.size)
        {}

        inline constexpr Vec getMin() const { return position - origin; }
        inline constexpr Vec getMax() const { return position - origin + size; }

        inline constexpr Num getMin(size_t const i) const { return position[i] - origin[i]; }
        inline constexpr Num getMax(size_t const i) const { return position[i] - origin[i] + size[i]; }

        inline Box& operator= (Box const& box)
        {
            position = box.position;
            size     = box.size;
            origin   = box.origin;
            return *this;
        }

        inline string_t str() const
        {
            return sprintTuple(position, size);
        }

        inline bool tryParse(Segment const& s)
        {
            return tryParseTuple(s, position, size, origin);
        }

        inline bool tryParse(istream_t& is)
        {
            return tryParseTuple(is, position, size, origin);
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Box)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Box)
    };
}
