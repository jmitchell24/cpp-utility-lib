// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "BoxN.hpp"
#include "../Vector3D.hpp"

namespace util
{
    template <typename N> class BoxND<N, 3>
    {
    public:
        typedef N          Num;
        typedef Vec3N<Num> Vec;
        typedef BoxND<N,3> Box;

        typedef best_param<Num> NumParam;
        typedef best_param<Vec> VecParam;
        typedef best_param<Box> BoxParam;


        size_t static constexpr Size = 3;

        union
        {
            struct
            {
                union
                {
                    struct { Num x, y, z; };
                    Vec position;
                };

                union
                {
                    struct { Num width, height, depth; };
                    Vec size;
                };
            };

            N components[sizeof(Vec)*3];
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

        inline constexpr BoxND(VecParam position, NumParam width, NumParam height, NumParam depth)
            : position(position), size(width,height,depth)
        {}

        inline constexpr BoxND(NumParam x, NumParam y, NumParam z, NumParam width, NumParam height, NumParam depth)
            : position(x,y,z), size(width,height,depth)
        {}

        inline constexpr BoxND(Box const& box)
            : position(box.position), size(box.size)
        {}

        template <typename P>
        inline explicit constexpr BoxND(BoxND<P,3> const& box)
            : position(box.position), size(box.size)
        {}

        inline constexpr Vec getMin() const { return position - origin; }
        inline constexpr Vec getMax() const { return position - origin + size; }

        inline constexpr Num getMinX() const { return position.x - origin.x; }
        inline constexpr Num getMinY() const { return position.y - origin.y; }
        inline constexpr Num getMinZ() const { return position.z - origin.z; }

        inline constexpr Num getMaxX() const { return position.x - origin.x + size.x; }
        inline constexpr Num getMaxY() const { return position.y - origin.y + size.y; }
        inline constexpr Num getMaxZ() const { return position.z - origin.z + size.z; }

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

    template<typename N> using Box3N = BoxND<N, 3>;

    typedef Box3N<real_t  > Box3;
    typedef Box3N<float   > FBox3;
    typedef Box3N<double  > DBox3;
    typedef Box3N<int     > IBox3;
    typedef Box3N<unsigned> UBox3;

    extern template struct BoxND<float   , 3>;
    extern template struct BoxND<double  , 3>;
    extern template struct BoxND<int     , 3>;
    extern template struct BoxND<unsigned, 3>;
}
