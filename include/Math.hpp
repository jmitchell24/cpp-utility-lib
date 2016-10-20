// Copyright 2013, James Mitchell, All rights reserved.

#ifndef MATH_UTIL
#define MATH_UTIL

#include "Types.hpp"

#include "math/Matrix4x4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector2D.hpp"
#include "math/Vector3D.hpp"
#include "math/Vector4D.hpp"
#include "math/ColorRGBA.hpp"

#include "math/collision/Box3.hpp"
#include "math/collision/Box2.hpp"
#include "math/collision/Circle.hpp"
#include "math/collision/Sphere.hpp"
#include "math/collision/Ray.hpp"
#include "math/collision/Triangle.hpp"

namespace util
{
/*
    inline real_tl_treal_t1.0)
    {
        return real_t2818284) * multiplier;
    }

    inline real_teal_treal_t1.0)
    {
        return real_t5926535) * multiplier;
    }
*/
    inline radians_t constexpr toRad(degrees_t degrees)
    {
        return (radians_t)57.2957795 * degrees;
    }

    inline degrees_t constexpr toDeg(radians_t radians)
    {
        return (degrees_t)0.0174532925 * radians;
    }
}

#endif
