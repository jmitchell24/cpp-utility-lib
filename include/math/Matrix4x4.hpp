// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../Types.hpp"
#include "../string/Types.hpp"
#include "../string/Segment.hpp"

#include "MatrixNxN.hpp"
#include "Vector4D.hpp"

/*----------
opengl matrix array index layout

| 0 4 8  12 |
| 1 5 9  13 |
| 2 6 10 14 |
| 3 7 11 15 |

----------*/

#define OrderedConstructMx44(m00,m01,m02,m03, \
                             m04,m05,m06,m07, \
                             m08,m09,m10,m11, \
                             m12,m13,m14,m15) \
Mat44(m00, m04, m08, m12, \
      m01, m05, m09, m13, \
      m02, m06, m10, m14, \
      m03, m07, m11, m15)

namespace util
{
    template <typename N> struct MatNRC<N,4,4>
    {
        typedef N               Num;
        typedef N&              NumRef;
        typedef best_param<Num> NumParam;

        typedef Vec3N<Num>       Vec3;
        typedef Vec3&            Vec3Ref;
        typedef best_param<Vec3> Vec3Param;

        typedef Vec4N<Num>       Vec4;
        typedef Vec4&            Vec4Ref;
        typedef best_param<Vec4> Vec4Param;

        typedef MatNRC<Num,4,4>   Mat44;
        typedef Mat44&            Mat44Ref;
        typedef best_param<Mat44> Mat44Param;

        typedef N                 Components[16];

        size_t static constexpr Rows    = 4;
        size_t static constexpr Columns = 4;
        size_t static constexpr Size    = Rows*Columns;

        union
        {
            Components m,c;
            Components components;

            struct
            {
                Num m11, m21, m31, m41,
                    m12, m22, m32, m42,
                    m13, m23, m33, m43,
                    m14, m24, m34, m44;
            };
        };

        inline MatNRC() :
            m11((Num)1), m21((Num)0), m31((Num)0), m41((Num)0),
            m12((Num)0), m22((Num)1), m32((Num)0), m42((Num)0),
            m13((Num)0), m23((Num)0), m33((Num)1), m43((Num)0),
            m14((Num)0), m24((Num)0), m34((Num)0), m44((Num)1)
        {}

        inline explicit MatNRC(NumParam n)
            : components{ n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n }
        {
            std::fill(begin(), end(), n);
        }

        inline MatNRC(NumParam m11, NumParam m12, NumParam m13, NumParam m14,
                      NumParam m21, NumParam m22, NumParam m23, NumParam m24,
                      NumParam m31, NumParam m32, NumParam m33, NumParam m34,
                      NumParam m41, NumParam m42, NumParam m43, NumParam m44)
            : components
                {
                    m11, m21, m31, m41,
                    m12, m22, m32, m42,
                    m13, m23, m33, m43,
                    m14, m24, m34, m44
                }
        {}

        inline explicit MatNRC(Mat44&& m)
            : MatNRC(std::move(m.components))
        {}

        inline MatNRC(Mat44 const& m)
            : MatNRC(m.components)
        {
            std::copy(std::begin(m.components), std::end(m.components), std::begin(components));
        }

        template <typename P>
        inline explicit MatNRC(MatNRC<P,4,4> const& m)
            : MatNRC(m.begin(), m.end())
        {}

        inline explicit MatNRC(Components&& components)
        {
            std::copy(std::begin(components), std::end(components), std::begin(this->components));
        }

        inline explicit MatNRC(Components const& components)
        {
            std::copy(std::begin(components), std::end(components), std::begin(this->components));
        }

        template <typename P>
        inline MatNRC(std::initializer_list<P> const& il)
            : MatNRC(il.begin(), il.end())
        {}

        template <typename I>
        inline MatNRC(I first, I last)
            : MatNRC(make_matrix<N,4,4>(first, last))
        {}

        inline bool isNan() const
        {
            for (unsigned i = 0; i < Size; ++i)
                if (std::isnan(components[i]))
                    return true;
            return false;
        }

        inline Num trace() const
        {
            return m[0] + m[5] + m[10] + m[15];
        }

        inline Mat44 transpose() const
        {
            return Mat44(m[ 0], m[ 1], m[ 2], m[ 3],
                         m[ 4], m[ 5], m[ 6], m[ 7],
                         m[ 8], m[ 9], m[10], m[11],
                         m[12], m[13], m[14], m[15]);
        }

        inline Vec3 transformedPoint(Vec3Param v) const
        {
            return Vec3(v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12],
                        v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13],
                        v.x*m[2] + v.y*m[6] + v.z*m[10]+ m[14]);
        }

        inline Vec3 transformedVector(Vec3Param v) const
        {
            return Vec3(v.x*m[0] + v.y*m[4] + v.z*m[8],
                        v.x*m[1] + v.y*m[5] + v.z*m[9],
                        v.x*m[2] + v.y*m[6] + v.z*m[10]);
        }

        Mat44 inverse() const
        {
            Components inv;

            inv[0]  =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
            inv[1]  = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
            inv[2]  =  m[1] * m[6]  * m[15] - m[1] * m[7]  * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7]  - m[13] * m[3] * m[6];
            inv[3]  = -m[1] * m[6]  * m[11] + m[1] * m[7]  * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9]  * m[2] * m[7]  + m[9]  * m[3] * m[6];
            inv[4]  = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
            inv[5]  =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
            inv[6]  = -m[0] * m[6]  * m[15] + m[0] * m[7]  * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7]  + m[12] * m[3] * m[6];
            inv[7]  =  m[0] * m[6]  * m[11] - m[0] * m[7]  * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8]  * m[2] * m[7]  - m[8]  * m[3] * m[6];
            inv[8]  =  m[4] * m[9]  * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
            inv[9]  = -m[0] * m[9]  * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
            inv[10] =  m[0] * m[5]  * m[15] - m[0] * m[7]  * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7]  - m[12] * m[3] * m[5];
            inv[12] = -m[4] * m[9]  * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
            inv[11] = -m[0] * m[5]  * m[11] + m[0] * m[7]  * m[9]  + m[4] * m[1] * m[11] - m[4] * m[3] * m[9]  - m[8]  * m[1] * m[7]  + m[8]  * m[3] * m[5];
            inv[13] =  m[0] * m[9]  * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
            inv[14] = -m[0] * m[5]  * m[14] + m[0] * m[6]  * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6]  + m[12] * m[2] * m[5];
            inv[15] =  m[0] * m[5]  * m[10] - m[0] * m[6]  * m[9]  - m[4] * m[1] * m[10] + m[4] * m[2] * m[9]  + m[8]  * m[1] * m[6]  - m[8]  * m[2] * m[5];

            Num det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

            if (carefulEqualityZero(det))
                return *this;

            det = (Num)1.0 / det;
            for (unsigned i = 0; i < Size; ++i)
                inv[i] *= det;
            return Mat44(inv);
        }

        inline static Mat44 identity()
        {
            return Mat44();
        }

        inline static Mat44 scale(NumParam x, NumParam y, NumParam z)
        {
            return Mat44(     x, (Num)0, (Num)0, (Num)0,
                         (Num)0,      y, (Num)0, (Num)0,
                         (Num)0, (Num)0,      z, (Num)0,
                         (Num)0, (Num)0, (Num)0, (Num)1 );
        }

        inline static Mat44 scale(Vec3Param v)
        {
            return scale(v.x, v.y, v.z);
        }

        inline static Mat44 translate(NumParam x, NumParam y, NumParam z)
        {
            return Mat44( (Num)1, (Num)0, (Num)0, x,
                          (Num)0, (Num)1, (Num)0, y,
                          (Num)0, (Num)0, (Num)1, z,
                          (Num)0, (Num)0, (Num)0, (Num)1);
        }

        inline static Mat44 translate(Vec3Param v)
        {
            return translate(v.x, v.y, v.z);
        }

        // x, y, z must be normalized
        inline static Mat44 rotate(NumParam angle, NumParam x, NumParam y, NumParam z)
        {
            Num const s = sin(angle);
            Num const c = cos(angle);

            return OrderedConstructMx44
            (
                c + (1-c) * x,
                (1-c) * x*y + s*z,
                (1-c) * x*z - s*y,
                0,
                (1-c) * y*x - s*z,
                c + (1-c) * y*y,
                (1-c) * y*z + s*x,
                0,
                (1-c) * z*x + s*y,
                (1-c) * z*z - s*x,
                c + (1-c) * (z*z),
                0,0,0,0,1
            );
        }

        inline static Mat44 rotate(NumParam angle, Vec3Param axis)
        {
            return rotate(angle, axis.x, axis.y, axis.z);
        }

        inline static Mat44 rotateX(NumParam angle)
        {
            Num const s = sin(angle);
            Num const c = cos(angle);

            return Mat44( (Num)1, (Num)0, (Num)0, (Num)0,
                          (Num)0,      c,     -s, (Num)0,
                          (Num)0,      s,      c, (Num)0,
                          (Num)0, (Num)0, (Num)0, (Num)1 );
        }

        inline static Mat44 rotateY(NumParam angle)
        {
            Num const s = sin(angle);
            Num const c = cos(angle);

            return Mat44(      c, (Num)0,      s, (Num)0,
                          (Num)0, (Num)1, (Num)0, (Num)0,
                              -s, (Num)0,      c, (Num)0,
                          (Num)0, (Num)0, (Num)0, (Num)1 );
        }

        inline static Mat44 rotateZ(NumParam angle)
        {
            Num const s = sin(angle);
            Num const c = cos(angle);

            return Mat44(      c,     -s, (Num)0, (Num)0,
                               s,      c, (Num)0, (Num)0,
                          (Num)0, (Num)0, (Num)1, (Num)0,
                          (Num)0, (Num)0, (Num)0, (Num)1 );
        }

        Mat44 rotation() const
        {
            return Mat44(  m[0],  m[4], m[8] , (Num)0,
                           m[1],  m[5], m[9] , (Num)0,
                           m[2],  m[6], m[10], (Num)0,
                         (Num)0,(Num)0,(Num)0, (Num)1);
        }

        Mat44 translation() const
        {
            return Mat44((Num)1, (Num)0, (Num)0, m[12],
                         (Num)0, (Num)1, (Num)0, m[13],
                         (Num)0, (Num)0, (Num)1, m[14],
                         (Num)0, (Num)0, (Num)0, (Num)1);
        }

        inline void setIdentity() { new (this) Mat44(identity()); }

        inline void setScale(Vec3Param v) { new (this) Mat44(scale(v)); }
        inline void setScale(NumParam x, NumParam y, NumParam z) { new (this) Mat44(scale(x,y,z)); }

        inline void setTranslate(Vec3Param v) { new (this) Mat44(translate(v)); }
        inline void setTranslate(NumParam x, NumParam y, NumParam z) { new (this) Mat44(translate(x,y,z)); }

        inline void setRotate(NumParam angle, Vec3Param axis) { new (this) Mat44(rotate(angle,axis)); }
        inline void setRotate(NumParam angle, NumParam x, NumParam y, NumParam z) { new (this) Mat44(rotate(angle,x,y,z)); }

        inline void setRotateX(NumParam angle) { new (this) Mat44(rotateX(angle)); }
        inline void setRotateY(NumParam angle) { new (this) Mat44(rotateY(angle)); }
        inline void setRotateZ(NumParam angle) { new (this) Mat44(rotateZ(angle)); }

        inline Mat44 operator + (Mat44Param m) const
        {
            Mat44 temp; // should be vectorized by gcc with flag -O3
            for (size_t i = 0; i < Size; ++i)
                temp.c[i] = c[i] + m.c[i];
            return temp;
        }

        inline Mat44 operator - (Mat44Param m) const
        {
            Mat44 temp;
            for (size_t i = 0; i < Size; ++i)
                temp.c[i] = c[i] - m.c[i];
            return temp;
        }

        inline Vec4 operator * (Vec4Param v) const
        {
            return Vec4(
            components[0]*v[0]+components[4]*v[1]+components[8 ]*v[2]+components[12]*v[3],
            components[1]*v[0]+components[5]*v[1]+components[9 ]*v[2]+components[13]*v[3],
            components[2]*v[0]+components[6]*v[1]+components[10]*v[2]+components[14]*v[3],
            components[3]*v[0]+components[7]*v[1]+components[11]*v[2]+components[15]*v[3]);
        }

        inline Mat44 operator * (Mat44Param m) const
        {
            return OrderedConstructMx44
            (
                (c[0]*m.c[ 0])+(c[4]*m.c[ 1])+(c[ 8]*m.c[ 2])+(c[12]*m.c[ 3]),
                (c[1]*m.c[ 0])+(c[5]*m.c[ 1])+(c[ 9]*m.c[ 2])+(c[13]*m.c[ 3]),
                (c[2]*m.c[ 0])+(c[6]*m.c[ 1])+(c[10]*m.c[ 2])+(c[14]*m.c[ 3]),
                (c[3]*m.c[ 0])+(c[7]*m.c[ 1])+(c[11]*m.c[ 2])+(c[15]*m.c[ 3]),
                (c[0]*m.c[ 4])+(c[4]*m.c[ 5])+(c[ 8]*m.c[ 6])+(c[12]*m.c[ 7]),
                (c[1]*m.c[ 4])+(c[5]*m.c[ 5])+(c[ 9]*m.c[ 6])+(c[13]*m.c[ 7]),
                (c[2]*m.c[ 4])+(c[6]*m.c[ 5])+(c[10]*m.c[ 6])+(c[14]*m.c[ 7]),
                (c[3]*m.c[ 4])+(c[7]*m.c[ 5])+(c[11]*m.c[ 6])+(c[15]*m.c[ 7]),
                (c[0]*m.c[ 8])+(c[4]*m.c[ 9])+(c[ 8]*m.c[10])+(c[12]*m.c[11]),
                (c[1]*m.c[ 8])+(c[5]*m.c[ 9])+(c[ 9]*m.c[10])+(c[13]*m.c[11]),
                (c[2]*m.c[ 8])+(c[6]*m.c[ 9])+(c[10]*m.c[10])+(c[14]*m.c[11]),
                (c[3]*m.c[ 8])+(c[7]*m.c[ 9])+(c[11]*m.c[10])+(c[15]*m.c[11]),
                (c[0]*m.c[12])+(c[4]*m.c[13])+(c[ 8]*m.c[14])+(c[12]*m.c[15]),
                (c[1]*m.c[12])+(c[5]*m.c[13])+(c[ 9]*m.c[14])+(c[13]*m.c[15]),
                (c[2]*m.c[12])+(c[6]*m.c[13])+(c[10]*m.c[14])+(c[14]*m.c[15]),
                (c[3]*m.c[12])+(c[7]*m.c[13])+(c[11]*m.c[14])+(c[15]*m.c[15])
            );
        }

        inline Mat44 operator + (NumParam n)
        {
            Mat44 temp;
            for (size_t i = 0; i < Size; ++i)
                temp.c[i] = c[i] + n;
            return temp;
        }

        inline Mat44 operator - (NumParam n)
        {
            Mat44 temp;
            for (size_t i = 0; i < Size; ++i)
                temp.c[i] = c[i] - n;
            return temp;
        }

        inline Mat44 operator * (NumParam n)
        {
            Mat44 temp;
            for (size_t i = 0; i < Size; ++i)
                temp.c[i] = c[i] * n;
            return temp;
        }

        inline Mat44 operator / (NumParam n)
        {
            Mat44 temp;
            for (size_t i = 0; i < Size; ++i)
                temp.c[i] = c[i] / n;
            return temp;
        }

        inline Mat44Ref operator  = (Mat44Param m) { return *new (this) Mat44(m); }
        inline Mat44Ref operator += (Mat44Param m) { return (*this = *this + m); }
        inline Mat44Ref operator -= (Mat44Param m) { return (*this = *this - m); }
        inline Mat44Ref operator *= (Mat44Param m) { return (*this = *this * m); }

        inline Mat44Ref operator += (NumParam n) { return (*this = *this + n); }
        inline Mat44Ref operator -= (NumParam n) { return (*this = *this - n); }
        inline Mat44Ref operator *= (NumParam n) { return (*this = *this * n); }
        inline Mat44Ref operator /= (NumParam n) { return (*this = *this / n); }

        inline bool operator== (Mat44Param m) const { return std::equal(begin(), end(), m.begin()); }
        inline bool operator!= (Mat44Param m) const { return !(*this == m); }
        inline bool operator<  (Mat44Param m) const { return std::lexicographical_compare(begin(), end(), m.begin(), m.end()); }
        inline bool operator>  (Mat44Param m) const { return *this < m; }
        inline bool operator<= (Mat44Param m) const { return !(*this < m); }
        inline bool operator>= (Mat44Param m) const { return !(m < *this); }

        inline NumParam operator[] (size_t const i) const { ASSERT(i < Size); return components[i]; }
        inline Num    & operator[] (size_t const i)       { ASSERT(i < Size); return components[i]; }

        inline Num*       begin()       { return std::begin(components); }
        inline Num const* begin() const { return std::begin(components); }

        inline Num*       end()       { return std::end(components); }
        inline Num const* end() const { return std::end(components); }

        string_t str() const
        {
            string_t strings[Size] =
            {
                toString(c[0]), toString(c[1]), toString(c[2]), toString(c[3]),
                toString(c[4]), toString(c[5]), toString(c[6]), toString(c[7]),
                toString(c[8]), toString(c[9]), toString(c[10]), toString(c[11]),
                toString(c[12]), toString(c[13]), toString(c[14]), toString(c[15])
            };

            return sprint(STR_LITERAL(
                          "([#,@16],[#,@17],[#,@18],[#,@19])\n"
                          "([#,@16],[#,@17],[#,@18],[#,@19])\n"
                          "([#,@16],[#,@17],[#,@18],[#,@19])\n"
                          "([#,@16],[#,@17],[#,@18],[#,@19])"),
            strings[ 0], strings[ 1], strings[ 2], strings[ 3],
            strings[ 4], strings[ 5], strings[ 6], strings[ 7],
            strings[ 8], strings[ 9], strings[10], strings[11],
            strings[12], strings[13], strings[14], strings[15],
            varargs::max(strings[0].size(), strings[4].size(), strings[8 ].size(), strings[12].size()),
            varargs::max(strings[1].size(), strings[5].size(), strings[9 ].size(), strings[13].size()),
            varargs::max(strings[2].size(), strings[6].size(), strings[10].size(), strings[14].size()),
            varargs::max(strings[3].size(), strings[7].size(), strings[11].size(), strings[15].size()));
        }

        DEFINE_OBJECT_OSTREAM_OPERATOR(Mat44)
        //DEFINE_OBJECT_ISTREAM_OPERATOR(Mat44)
    };

    template <typename N> using Mat44N = MatNRC<N,4,4>;

    typedef Mat44N<real_t> Mat44;
    typedef Mat44N<float>  Mat44f;
    typedef Mat44N<double> Mat44d;

    extern template struct MatNRC<float,4,4>;
    extern template struct MatNRC<double,4,4>;
}

#undef OrderedConstructMx44
