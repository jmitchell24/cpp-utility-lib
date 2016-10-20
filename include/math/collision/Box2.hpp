// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "BoxN.hpp"
#include "../Vector2D.hpp"

namespace util
{
    template <typename N> class BoxND<N, 2>
    {
    public:
        typedef N          Num;
        typedef Vec2N<Num> Vec;
        typedef BoxND<N,2> Box;

        typedef best_param<Num> NumParam;
        typedef best_param<Vec> VecParam;
        typedef best_param<Box> BoxParam;

        size_t static constexpr Size = 2;

        union
        {
            struct
            {
                union
                {
                    struct { Num x, y; };
                    Vec position;
                };

                union
                {
                    struct { Num width, height; };
                    Vec size;
                };
            };

            N components[sizeof(Vec)*2];
        };

        Vec origin;

        inline constexpr BoxND()
            : position(), size()
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
        inline explicit constexpr BoxND(BoxND<P,2> const& box)
            : position((Vec)box.position), size((Vec)box.size)
        {}

        inline constexpr Vec getMin() const { return position - origin; }
        inline constexpr Vec getMax() const { return position - origin + size; }

        inline constexpr Num getMinX() const { return position.x - origin.x; }
        inline constexpr Num getMinY() const { return position.y - origin.y; }
        inline constexpr Num getMaxX() const { return position.x - origin.x + size.x; }
        inline constexpr Num getMaxY() const { return position.y - origin.y + size.y; }

        inline Box& operator= (Box const& box)
        {
            position = box.position;
            size     = box.size;
            origin   = box.origin;
            return *this;
        }

        inline string_t str() const
        {
            return sprintTuple(position, size, origin);
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

    template<typename N> using RectN = BoxND<N, 2>;
    template<typename N> using Box2N = BoxND<N, 2>;

    typedef Box2N<real_t  > Box2;
    typedef Box2N<float   > FBox2;
    typedef Box2N<double  > DBox2;
    typedef Box2N<int     > IBox2;
    typedef Box2N<unsigned> UBox2;

    typedef Box2N<real_t  > Rect;
    typedef Box2N<float   > FRect;
    typedef Box2N<double  > DRect;
    typedef Box2N<int     > IRect;
    typedef Box2N<unsigned> URect;

    extern template struct BoxND<float   , 2>;
    extern template struct BoxND<double  , 2>;
    extern template struct BoxND<int     , 2>;
    extern template struct BoxND<unsigned, 2>;
}
