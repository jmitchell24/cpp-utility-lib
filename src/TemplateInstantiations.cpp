// Copyright 2015, James Mitchell, All rights reserved.

#include "All.hpp"

namespace util
{
    template struct VecND<float        , 4>;
    template struct VecND<double       , 4>;
    template struct VecND<int          , 4>;
    template struct VecND<unsigned     , 4>;
    template struct VecND<unsigned char, 4>;

    template struct VecND<float        , 3>;
    template struct VecND<double       , 3>;
    template struct VecND<int          , 3>;
    template struct VecND<unsigned     , 3>;
    template struct VecND<unsigned char, 3>;

    template struct VecND<float        , 2>;
    template struct VecND<double       , 2>;
    template struct VecND<int          , 2>;
    template struct VecND<unsigned     , 2>;
    template struct VecND<unsigned char, 2>;

    template struct MatNRC<float ,4,4>;
    template struct MatNRC<double,4,4>;

    template struct QuatN<float>;
    template struct QuatN<double>;

    template struct RayN<float   >;
    template struct RayN<double  >;
    template struct RayN<int     >;
    template struct RayN<unsigned>;

    template struct BoundND<float   , 2>;
    template struct BoundND<double  , 2>;
    template struct BoundND<int     , 2>;
    template struct BoundND<unsigned, 2>;

    template struct BoundND<float   , 3>;
    template struct BoundND<double  , 3>;
    template struct BoundND<int     , 3>;
    template struct BoundND<unsigned, 3>;

    template struct BoxND<float   , 2>;
    template struct BoxND<double  , 2>;
    template struct BoxND<int     , 2>;
    template struct BoxND<unsigned, 2>;

    template struct BoxND<float   , 3>;
    template struct BoxND<double  , 3>;
    template struct BoxND<int     , 3>;
    template struct BoxND<unsigned, 3>;

    template struct CircleN<float   >;
    template struct CircleN<double  >;
    template struct CircleN<int     >;
    template struct CircleN<unsigned>;

    template struct TPlane<float   >;
    template struct TPlane<double  >;
    template struct TPlane<int     >;
    template struct TPlane<unsigned>;

    template struct SphereN<float   >;
    template struct SphereN<double  >;
    template struct SphereN<int     >;
    template struct SphereN<unsigned>;
}
